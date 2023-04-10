#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string>
#include <utility>
#include <cmath>

#include <sys/stat.h>
#include "xxhash.h"

#include "utils.h"


// ----------------- File Utils -----------------------

bool file_utils::sst_filename_compare(std::string file_a, std::string file_b) {
  int file_a_age = std::stoi(file_a.substr(4));
  int file_b_age = std::stoi(file_b.substr(4));
  return file_a_age < file_b_age;
}

long file_utils::get_filesize(std::string filepath) {
  struct stat file_info;
  if (stat(filepath.c_str(), &file_info) == -1) {
    perror("Error reading file info");
    return -1;
  }
  long filesize = file_info.st_size;
  return filesize;
}


std::vector<std::pair<long, long>> file_utils::reintrepret_buffer(void *buffer, int bytes_read) {
  char *buffer_char = (char *) buffer;
  std::vector<std::pair<long, long> > values (reinterpret_cast<std::pair<long, long>*>(buffer_char), 
                                             reinterpret_cast<std::pair<long, long>*>(buffer_char + bytes_read));

  return values;
}

int file_utils::read_block(void * &buffer, std::string filepath, long block_num) {
  int fd = open(filepath.c_str(), O_RDONLY | O_CREAT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  long offset = block_num * BLOCK_SIZE;
  long bytes_read = pread(fd, buffer, BLOCK_SIZE, offset);
                        
  close(fd);

  return bytes_read;
}

int file_utils::read_block_buffer_pool(BufferPool *buffer_pool, void * &buffer, std::string filepath, long block_num) {
  int fd = open(filepath.c_str(), O_RDONLY | O_CREAT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  int bytes_read;

  std::string hash_key = filepath + "_" + std::to_string(block_num);

  int found = buffer_pool->get_data(hash_key, buffer, bytes_read);

  if(found == 0) {
    close(fd);
    return bytes_read;
  }

  long offset = block_num * BLOCK_SIZE;
  bytes_read = pread(fd, buffer, BLOCK_SIZE, offset);

  buffer_pool->insert_data(hash_key, buffer, bytes_read);

  close(fd);
  return bytes_read;
}


int file_utils::write_data(std::string filepath, long* data, long data_size) {
  int fd = open(filepath.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666); // Add O_DIRECT Flag

  if (fd == -1) {
      perror("Error opening file");
      return -1;
  }

  long num_blocks = data_size / BLOCK_SIZE; 

  off_t offset = lseek(fd, 0, SEEK_END);
  if (offset == -1) {
      // error handling
      close(fd);
      return 1;
  }

  long size_unwritten = data_size;
  long* buf_unwritten = data;
  const int block_size_long = BLOCK_SIZE / sizeof(long);

  for (long i = 0; i < num_blocks; i++) {
    // Equivalent to 1 I/O
    if(pwrite(fd, buf_unwritten, BLOCK_SIZE, offset) == -1) {
      perror("Error writing to file");
      return -1;
    } 
    offset += BLOCK_SIZE;
    size_unwritten -= BLOCK_SIZE;
    buf_unwritten += block_size_long;
  }

  if(size_unwritten > 0) {
    if(pwrite(fd, buf_unwritten, size_unwritten, offset) == -1) {
      perror("Error writing to file");
      return -1;
    }
  }

  close(fd);

  return 0;
}



// ----------------- Search Utils -----------------------

long search_utils::binary_search_blocks(std::string filepath, long num_blocks, long key) {

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  long min_key_block;
  long max_key_block;

  void *buffer;
  int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }
  int bytes_read;

  std::vector<std::pair<long, long> > values;
  

  while (start_block <= end_block) {
    block_to_read = start_block + ((end_block - start_block) / 2);

    bytes_read = file_utils::read_block(buffer, filepath, block_to_read);
    values = file_utils::reintrepret_buffer(buffer, bytes_read);

    min_key_block = values[0].first;
    max_key_block = values.back().first;
    if (key < min_key_block ) {
      end_block = block_to_read - 1;
    } else if (key > max_key_block) {
      start_block = block_to_read + 1;
    } else {
      free(buffer);
      return block_to_read;
    }
  }
  free(buffer);
  return -1;
}

long search_utils::binary_search_range_blocks(std::string filepath, long num_blocks, long key1, long key2) {

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  long min_key_block;
  long max_key_block;

  void *buffer;
  int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }
  int bytes_read = 0;

  std::vector<std::pair<long, long> > values;
  
  while (start_block <= end_block) {
    block_to_read = start_block + ((end_block - start_block) / 2);
    
    bytes_read = file_utils::read_block(buffer, filepath, block_to_read);
    values = file_utils::reintrepret_buffer(buffer, bytes_read);

    min_key_block = values[0].first;
    max_key_block = values.back().first;
    if (key2 < min_key_block) {
      // if the upper key is less than the smallest key, need to search lower blocks
      end_block = block_to_read - 1;
    } else if (key1 > max_key_block) {
      // if the lower key is greater than the largest key, need to search higher blocks
      start_block = block_to_read + 1;
    } else {
      free(buffer);
      return start_block;
    }
  }
  free(buffer);
  return -1;
}

long search_utils::binary_search_kv(std::vector<std::pair<long, long> > key_values, long key) {
  long mid_index;
  long mid_key;
  long start_index = 0;
  long end_index = key_values.size()-1; // TODO: This is a bug, the actually size of key_values maybe smaller, then the binary search won't work 

  while (start_index <= end_index) {
    mid_index = start_index + ((end_index - start_index) / 2);
    mid_key = key_values[mid_index].first;

    if (key < mid_key) {
      end_index = mid_index - 1; 
    } else if (key > mid_key) {
      start_index = mid_index + 1;
    } else {
      return mid_index;
    }
  }
  return -1;
}



// ----------------- Hash Utils -----------------------

int hash_utils::get_hash_value(std::string hash_key, int seed) {
  return XXH32(hash_key.c_str(), hash_key.length(), seed);
}



// ----------------- Math Utils -----------------------

int math_utils::get_num_bits(unsigned int value) {
  if (value == 0) {
    return 1;
  }
  return floor(log2(value-1)) + 1;
}

int math_utils::get_prefix_bits(unsigned int value, unsigned int num_bits) {
  unsigned int val_num_bits = 32; // using 32 bit hash values
  if(val_num_bits > num_bits) {
    unsigned int diff = val_num_bits - num_bits;
    unsigned int mask = ((1 << num_bits) - 1);
    return (value >> diff) & mask;
  }
  return value;
}



// ----------------- Buffer Pool Utils -----------------------

int buffer_pool_utils::get_offset(int prefix_length, unsigned int hash_value) {
  return math_utils::get_prefix_bits(hash_value, prefix_length);
}

Bucket * buffer_pool_utils::get_bucket(const std::vector<Bucket *> directory, int prefix_length, std::string hash_key) {
  unsigned int hash_value = hash_utils::get_hash_value(hash_key, HASH_FUNC_SEED);
  unsigned int offset = buffer_pool_utils::get_offset(prefix_length, hash_value);
  Bucket *bucket = directory[offset];
  return bucket;
}

int buffer_pool_utils::insert_frame(Bucket *bucket, Frame *frame_to_insert) {
  if(bucket->frame_count == 0) {
    // Empty Bucket, initialize linked list
    bucket->frame = frame_to_insert;
    bucket->frame_count++;
    
    return 0;
  } else if(bucket->frame_count > 0) {
    
    // Collision, add to end of the list
    Frame *curr_frame = bucket->frame;

    while(curr_frame->next != nullptr) {
      if(curr_frame == frame_to_insert) {
        return -1;
      }
      curr_frame = curr_frame->next;
    }
    
    curr_frame->next = frame_to_insert;
    bucket->frame_count++;

    return 0;
    
  }
  return -1;
}

int buffer_pool_utils::delete_frame(std::vector<Bucket *> directory, int directory_size, int &dir_size_bytes, Bucket *bucket, Frame *frame) {
  Frame *curr_frame = bucket->frame;
  bool found_frame = false;

  if(curr_frame == frame) {
    bucket->frame = curr_frame->next;
    bucket->frame_count--;
    found_frame = true;

    // Other buckets may be pointing to the same frame
    Bucket *potential_bucket;
    for(int i = 0; i < directory_size; ++i) {
      potential_bucket = directory[i];
      if(potential_bucket->frame_count > 0 && potential_bucket->frame == frame) {
        potential_bucket->frame = curr_frame->next;
        potential_bucket->frame_count--;
      }
    }
  }

  while(curr_frame->next != nullptr) {
    if (curr_frame->next == frame) {
      curr_frame->next = frame->next;
      bucket->frame_count--;
      found_frame = true;
      break;
    }
    curr_frame = curr_frame->next;
  }

  if(found_frame) {
    dir_size_bytes -= frame->data_size;
    free(frame->data);
    frame->data = nullptr;
    delete frame;
  }

  return 0;
}

void buffer_pool_utils::find_next_frame(const std::vector<Bucket *> directory, int directory_size, Frame * &frame, int &bucket_num) {
  Frame *curr_frame = frame;
  int curr_bucket_num = bucket_num;
  Bucket *curr_bucket = directory[curr_bucket_num];

  while (curr_frame == frame) {
    // Find next frame in the "clock"
    if(curr_frame->next == nullptr) {
      // Find the next non-empty bucket
      do {
        // Wrap around 
        if(curr_bucket_num == (directory_size-1)) {
          curr_bucket_num = 0;
        } else {
          curr_bucket_num++;
        }
        curr_bucket = directory[curr_bucket_num];
      } while (curr_bucket->frame_count == 0);
      curr_frame = curr_bucket->frame;
      
    } else {
      curr_frame = curr_frame->next;
      break;
    }
  }
  bucket_num = curr_bucket_num;
  frame = curr_frame;
  
}


int buffer_pool_utils::rehash_bucket(std::vector<Bucket *> directory, int directory_size, int prefix_length, Bucket *bucket) {
  
  Frame *curr_frame = bucket->frame;

  // Find all buckets point to same list of frames, each will now point to a new bucket
  Bucket *potential_matching_bucket;
  for(int i = 0; i < directory_size; ++i) {
    potential_matching_bucket = directory[i];
    if(potential_matching_bucket->frame == curr_frame) {
      // Both buckets point to same node, split
      potential_matching_bucket->frame = nullptr;
      potential_matching_bucket->frame_count = 0;
      potential_matching_bucket->rehashed = true;
    }
  }

  Frame *frame_to_rehash;
  while(curr_frame != nullptr) {

    frame_to_rehash = curr_frame;
    curr_frame = curr_frame->next;

    frame_to_rehash->next = nullptr;

    Bucket *insert_into_bucket = buffer_pool_utils::get_bucket(directory, prefix_length, frame_to_rehash->hash_key);
    buffer_pool_utils::insert_frame(insert_into_bucket, frame_to_rehash);
  }
  return 0;
}

// ----------------- Options Utils -----------------------

LevelLSMOptions options_utils::storage_to_level(StorageOptions options, int level_num) {
  LevelLSMOptions level_LSM_options;
  level_LSM_options.db_name = options.db_name;
  level_LSM_options.level_num = level_num;
  level_LSM_options.sst_capacity = options.memtable_capacity;
  level_LSM_options.use_bloom_filters = options.use_bloom_filters;
  level_LSM_options.bloom_filter_options = options.bloom_filter_options;
  level_LSM_options.sst_structure = options.sst_structure;
  level_LSM_options.use_buffer_pool = options.use_buffer_pool;
  level_LSM_options.buffer_pool = options.buffer_pool;

  return level_LSM_options;
}

LevelLSMOptions options_utils::level_to_next_level(LevelLSMOptions options, int size_ratio) {
  LevelLSMOptions level_LSM_options;
  level_LSM_options.db_name = options.db_name;
  level_LSM_options.level_num = options.level_num + 1;
  level_LSM_options.sst_capacity = options.sst_capacity * size_ratio;
  level_LSM_options.use_bloom_filters = options.use_bloom_filters;
  level_LSM_options.bloom_filter_options = options.bloom_filter_options;
  level_LSM_options.sst_structure = options.sst_structure;
  level_LSM_options.use_buffer_pool = options.use_buffer_pool;
  level_LSM_options.buffer_pool = options.buffer_pool;

  return level_LSM_options;
}

SSTOptions options_utils::level_to_sst(LevelLSMOptions options, std::string filepath, int sst_capacity, bool file_exists) {
  SSTOptions sst_options;
  sst_options.use_bloom_filter = options.use_bloom_filters;
  sst_options.bloom_filter_options = options.bloom_filter_options;
  sst_options.filepath = filepath;
  sst_options.file_exists = file_exists;
  sst_options.sst_capacity = sst_capacity;
  sst_options.use_buffer_pool = options.use_buffer_pool;
  sst_options.buffer_pool = options.buffer_pool;  

  return sst_options;
}

// ----------------- Sort Utils -----------------------

bool sort_utils::compare_kv_pair(const std::pair<long, long>& a, const std::pair<long, long>& b) {
  return a.first < b.first;
}


// ----------------- Bloom Filter Utils -----------------------

int bloom_filter_utils::set_bloom_filter_params(int num_keys, double false_positive_rate, int &num_bits, int &num_hash_funcs) {
  // Using https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions to determine the optimal number of bits and hash functions to use
  int n = num_keys;

  num_bits = (-1 * n * log(false_positive_rate)) / (log(2) / 2);

  int m = num_bits;
  int k = (m / n) * log(2);

  num_hash_funcs = k;

  return 0;
}

int bloom_filter_utils::set_bloom_filter_params(int num_keys, int bits_per_entry, int &num_bits, int &num_hash_funcs) {
  int n = num_keys;

  num_bits = n * bits_per_entry;

  int m = num_bits;
  int k = (m / n) * log(2);

  num_hash_funcs = k;

  return 0;
}

