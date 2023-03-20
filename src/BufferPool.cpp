#include <iostream>
#include <vector>

#include "BufferPool.h"
#include "utils.h"


Bucket::Bucket(Frame *frame, int frame_count, bool rehashed) : frame(frame), frame_count(frame_count), rehashed(rehashed) {}

Frame::Frame(std::string hash_key, void *data) : hash_key(hash_key), data(data) {
  next = nullptr;
  prev = nullptr;
}

BufferPool::BufferPool(int initial_size, int max_size) {
  num_buckets = 0;
  dir_size_bytes = 0;
  num_frames = 0;
  max_num_buckets = max_size;

  // Initialize the directory
  int code = initialize_directory(initial_size);

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

int BufferPool::get_offset(unsigned int hash_value) {
  return math_utils::get_prefix_bits(hash_value, prefix_length);
}

Bucket * BufferPool::get_bucket(std::string hash_key) {
  unsigned int hash_value = hash_utils::get_hash_value(hash_key, HASH_FUNC_SEED);
  unsigned int offset = BufferPool::get_offset(hash_value);
  Bucket *bucket = directory[offset];
  return bucket;
}

bool BufferPool::expansion_required(float threshold) {
  float load_factor = (num_frames + 1) / num_buckets;
  return load_factor >= threshold;
}

int BufferPool::expand_directory() {

  int expanded_num_buckets = num_buckets * 2;
  directory.resize(expanded_num_buckets, nullptr);
  

  Bucket *bucket;
  Bucket *new_bucket;

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



int insert_frame(Bucket *bucket, Frame *frame_to_insert) {
  if(bucket->frame_count == 0) {
    // Empty Bucket, initialize linked list
    bucket->frame = frame_to_insert;
    bucket->frame_count++;
    return 0;
  } else if(bucket->frame_count > 0) {
    
    // Collision, add to end of the list
    
    Frame *curr_frame = bucket->frame;
    while(curr_frame->next != nullptr) {
      curr_frame = curr_frame->next;
    }
    
    // frame_to_insert->prev = curr_frame;
    curr_frame->next = frame_to_insert;
    
    bucket->frame_count++;
    return 0;
    
  }
  return -1;
}

int BufferPool::rehash_bucket(Bucket *bucket) {

  if((bucket->frame_count <= 1) || (bucket->rehashed)) {
    // This bucket does not need to be rehashed, 
    // it either has only 1 or no frames, or it has already been rehashed since the last expansion
    return -1;
  }
  
  bucket->rehashed = true;
  Frame *curr_frame = bucket->frame;

  // Find all buckets point to same list of frames, each will now point 
  Bucket *potential_matching_bucket;
  for(int i = 0; i < num_buckets; ++i) {
    potential_matching_bucket = directory[i];
    if(potential_matching_bucket->frame == curr_frame) {
      // Both buckets point to same node, split
      potential_matching_bucket->frame = nullptr;
      potential_matching_bucket->frame_count = 0;
      potential_matching_bucket->rehashed = true;
    }
  }

  // Rehash all frames
  Frame *frame_to_rehash;
  while(curr_frame != nullptr) {
    frame_to_rehash = curr_frame;
    curr_frame = curr_frame->next;

    frame_to_rehash->next = nullptr;
    frame_to_rehash->prev = nullptr;

    Bucket *insert_into_bucket = get_bucket(frame_to_rehash->hash_key);

    insert_frame(insert_into_bucket, frame_to_rehash);

  }
  
  return 0;
}

int BufferPool::insert_data(std::string hash_key, void *data) {

  // Check if insertion would cause an expansion

  bool expand = BufferPool::expansion_required(1);
  
  // If expansion criteria met and directory size is currently < max directory size, expand 
  if(expand) {
      
    if (num_buckets < max_num_buckets) {
      int code = BufferPool::expand_directory();
      if(code == -1) {
        perror("Unable to insert data into buffer pool");
        return -1;
      }
    } else {
      // Expansion criteria is met but directory reached max size, evict frame
      
      // decrement num_frames

    }
    
  }

  Bucket *insert_into_bucket = get_bucket(hash_key);
  Frame *frame_to_insert = new Frame(hash_key, data);


  insert_frame(insert_into_bucket, frame_to_insert);

  // increment num_frames
  num_frames++;

  // potentially rehash bucket
  rehash_bucket(insert_into_bucket);
  

}

int search_bucket(void * &data, const Bucket *bucket, std::string hash_key) {
  Frame *curr_frame = bucket->frame;
  while(curr_frame != nullptr) {
    if(curr_frame->hash_key == hash_key) {
      data = curr_frame->data;
      return 0;
    }
    curr_frame = curr_frame->next;
  }
  return -1;
}


int BufferPool::get_data(void * &data, std::string hash_key) {
  Bucket *bucket = get_bucket(hash_key);

  // TODO: rehash before or after search ???
  rehash_bucket(bucket);

  int found = search_bucket(data, bucket, hash_key);

  if(found == 0) {
    return 0;
  }
  return -1;
}



void BufferPool::print_bufpool() {
  for(int i = 0; i < num_buckets; ++i) {
    std::cout << "Bucket: " << i << "  Num Frames: "<< directory[i]->frame_count << " Rehashed: " << directory[i]->rehashed << std::endl;
    
  }
}