#include <iostream>
#include <vector>

#include "BufferPool.h"
#include "utils.h"


Bucket::Bucket(Frame *frame, int frame_count, bool rehashed) : frame(frame), frame_count(frame_count), rehashed(rehashed) {}

Frame::Frame(std::string hash_key, void *frame_data, int frame_data_size) : hash_key(hash_key) {
  data = frame_data;
  data_size = frame_data_size;
  next = nullptr;
  eviction_metadata = nullptr;
}

BufferPool::BufferPool(BufferPoolOptions options) {
  num_buckets = 0;
  dir_size_bytes = 0;
  num_frames = 0;
  max_num_buckets = options.max_size;

  // Initialize Eviction Policy
  if (options.evict_policy == CLOCK_EVICT) {
    eviction_policy = new ClockEvictionPolicy();
  } else if(options.evict_policy == LRU_EVICT) {
    eviction_policy = new LRUEvictionPolicy();
  }

  // Initialize the directory
  int code = initialize_directory(options.initial_size);

  // Number of bits
  prefix_length = math_utils::get_num_bits(num_buckets);  

}

int BufferPool::initialize_directory(int inital_num_buckets) {
  if (inital_num_buckets > max_num_buckets) {
    perror("Invalid initial size");
    return -1;
  }

  assert(directory.size() == 0);

  // Allocate sizeof(Bucket *) * inital_size memory 
  directory.reserve(inital_num_buckets);
  num_buckets = inital_num_buckets;
  dir_size_bytes = sizeof(Bucket *) * inital_num_buckets;

  
  Bucket *bucket_to_intialize;
  for(int i = 0; i < num_buckets; ++i) {
    bucket_to_intialize = new Bucket(nullptr, 0, true);
    directory.push_back(bucket_to_intialize);
  }

  return 0;
}

bool BufferPool::expansion_required(float threshold) {
  // Load Factor is the number of frames per bucket, threshold dictates when directory should be expanded
  float load_factor = (num_frames ) / num_buckets;
  return load_factor >= threshold;
}

int BufferPool::expand_directory() {

  int expanded_num_buckets = num_buckets * 2;

  // Expand the size of the directory
  directory.resize(expanded_num_buckets, nullptr);

  Bucket *bucket;
  Bucket *new_bucket;

  // Ensure all new buckets point to existing buckets
  for(int offset = num_buckets-1; offset >= 0; --offset) {
    
    bucket = directory[offset];

    int new_offset = offset << 1;
    new_bucket = new Bucket(bucket->frame, bucket->frame_count, false);

    bucket->rehashed = false;
    
    // If an exisitng bucket already exists, replace bucket
    if(directory[new_offset] != nullptr) {
      delete directory[new_offset];
    }

    directory[new_offset] = new_bucket;
   
  }

  for (int offset = 0; offset < expanded_num_buckets; offset += 2) {
    bucket = directory[offset];
    int new_offset = offset + 1;
    new_bucket = new Bucket(bucket->frame, bucket->frame_count, false);

    // Copy bucket
    bucket->rehashed = false;

    // If an exisitng bucket already exists, replace bucket
    if(directory[new_offset] != nullptr) {
      delete directory[new_offset];
    }

    directory[new_offset] = new_bucket;

  }

  num_buckets = expanded_num_buckets;
  prefix_length++;
  return 0;
}


int BufferPool::rehash_bucket(Bucket *bucket) {

  if((bucket->frame_count <= 1) || (bucket->rehashed)) {
    // This bucket does not need to be rehashed, 
    // it either has only 1 or no frames, or it has already been rehashed since the last expansion
    return -1;
  }
  
  bucket->rehashed = true;
  buffer_pool_utils::rehash_bucket(directory, num_buckets, prefix_length, bucket);
  return 0;
}

int BufferPool::insert_data(std::string hash_key, void *data, int data_size) {

  // Check if insertion would cause an expansion, pass in load factor
  bool expand = BufferPool::expansion_required(LOAD_FACTOR_THRESHOLD);
  
  // If expansion criteria met and directory size is currently < max directory size, expand 
  if(expand) {
    if (num_buckets < max_num_buckets) {
      // There is room for expansion, increase directory size 2x 
      int code = BufferPool::expand_directory();
      if(code == -1) {
        perror("Unable to insert data into buffer pool");
        return -1;
      }
      
    } else {
      // Expansion criteria is met but directory reached max size, means directory is at max capacity
      // Evict frame
      eviction_policy->evict_frame(directory, num_buckets, prefix_length);
      num_frames--;
    }
  }

  Bucket *insert_into_bucket = buffer_pool_utils::get_bucket(directory, prefix_length, hash_key);

  void *data_cpy;
  int code = posix_memalign(&data_cpy, BLOCK_SIZE, data_size);
  if(code != 0) {
    perror("Unable to allocate memory for Buffer Pool");
    return -1;
  }

  memcpy(data_cpy, data, data_size);
  Frame *frame_to_insert = new Frame(hash_key, data_cpy, data_size);

  buffer_pool_utils::insert_frame(insert_into_bucket, frame_to_insert);

  // increment num_frames
  num_frames++;

  eviction_policy->frame_created(frame_to_insert);

  // potentially rehash bucket
  rehash_bucket(insert_into_bucket);
}

int search_bucket(Frame * &frame, const Bucket *bucket, std::string hash_key) {
  Frame *curr_frame = bucket->frame;
  while(curr_frame != nullptr) {
    if(curr_frame->hash_key == hash_key) {
      frame = curr_frame;
      return 0;
    }
    curr_frame = curr_frame->next;
  }
  return -1;
}


int BufferPool::get_data(std::string hash_key, void * &data, int &data_size) {
  Bucket *bucket = buffer_pool_utils::get_bucket(directory, prefix_length, hash_key);

  rehash_bucket(bucket);

  Frame *found_frame;

  int found = search_bucket(found_frame, bucket, hash_key);

  if(found == 0) {
    eviction_policy->frame_accessed(found_frame);
    
    // data = found_frame->data;
    data_size = found_frame->data_size;
    memcpy(data, found_frame->data, data_size);
    return 0;
  }
  return -1;
}

int BufferPool::update_directory_size(int new_max_size) {
  if(new_max_size < num_buckets) {
    // Shrink directory
    while (new_max_size < num_buckets) {
      int new_dir_size = num_buckets / 2;
      int max_new_num_frames = new_dir_size * LOAD_FACTOR_THRESHOLD;

      int num_frames_to_evict = num_frames - max_new_num_frames;

      // Evict frames
      for (int i = 0; i < num_frames_to_evict; ++i) {
        eviction_policy->evict_frame(directory, num_buckets, prefix_length);
        num_frames--;
      }

      int new_prefix_length = prefix_length - 1;
      
      // Rehash all the frames in the buckets to removed after directory shrinks
      for (int bucket_index = 0; bucket_index < num_buckets; ++bucket_index) {
        Bucket *bucket = directory[bucket_index];
        buffer_pool_utils::rehash_bucket(directory, num_buckets, new_prefix_length, bucket);
      }

      for (int bucket_index = new_dir_size; bucket_index < num_buckets; ++bucket_index) {
        Bucket *bucket = directory[bucket_index];
        bucket->frame = nullptr;
        delete bucket;
        directory[bucket_index] = nullptr;
      }

      directory.resize(new_dir_size);
      prefix_length--;
      num_buckets = new_dir_size;
    }
  }
  max_num_buckets = new_max_size;
}

void BufferPool::reset_buffer_pool() {
  for (int bucket_index = 0; bucket_index < num_buckets; ++bucket_index) {
    Bucket *bucket = directory[bucket_index];

    Frame *curr = bucket->frame;
    while (curr != nullptr) {
      Frame *frame_to_delete = curr;
      curr = curr->next;
      free(frame_to_delete->data);
      frame_to_delete->data = nullptr;
      delete frame_to_delete;
    }

    bucket->frame = nullptr;
    delete bucket;
    directory[bucket_index] = nullptr;
  }
}

// TODO: For DEBUGGING purposes only, REMOVE
void BufferPool::print_bufpool() {
  for(int i = 0; i < num_buckets; ++i) {
    std::cout << "Bucket: " << i << "  Num Frames: "<< directory[i]->frame_count << " Rehashed: " << directory[i]->rehashed << std::endl;
    bool x = false;
    Frame *curr = directory[i]->frame;
    while(curr != nullptr) {
      x = true;
      std::cout << "   " << curr->hash_key << " <-- ";
      curr = curr->next;
    }
    if(x) {
      std::cout << std::endl;
    }
  }
   std::cout << "Number of frames: " << num_frames << std::endl;
}