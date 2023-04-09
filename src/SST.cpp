#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string>
#include <utility>
#include <cstdio>

#include <sys/stat.h>

#include "SST.h"
#include "utils.h"


ListSST::ListSST(SSTOptions sst_options) {
  options = sst_options;
  bloom_filter = nullptr;
  bloom_filter_loaded = false;
  if (sst_options.use_bloom_filter) {
    bloom_filter = new BloomFilter(options.sst_capacity, options.bloom_filter_options);
  }
}

int ListSST::write_to_file(std::vector<std::pair<long, long>> data) {
  
  if(!options.file_exists) {
    int code;
    long *data_buffer = reinterpret_cast<long*>(data.data());
    long data_size = data.size() * 2 * sizeof(long);

    if(options.use_bloom_filter) {
      
      bloom_filter->insert_keys(data);
      bloom_filter_loaded = true;

      // // First block is metadata
      // int offset = 1;
      // int metadata_block_size = BLOCK_SIZE / sizeof(long);
      // // long* metadata_block = new long[metadata_block_size];
      // std::vector<int> metadata_block(metadata_block_size, 0);
      
      // std::vector<unsigned long> bit_array;
      // bloom_filter->convert_bits_to_long_array(bit_array);

      // // end block bloom bits
      // int bit_array_size = sizeof(long) * bit_array.size();
      // int num_blocks_bits = (BLOCK_SIZE + bit_array_size - 1) / bit_array_size;
      // metadata_block[0] = offset + num_blocks_bits;

      // // num bloom bits
      // metadata_block[1] = bloom_filter->num_bits;

      // offset += num_blocks_bits;

      // // end block bloom seeds
      // int seeds_array_size = sizeof(long) * bloom_filter->num_hash_funcs;
      // int num_blocks_seeds = (BLOCK_SIZE + seeds_array_size - 1) / seeds_array_size;
      // metadata_block[2] = bloom_filter->num_bits;

      // // num hash seeds
      // metadata_block[3] = bloom_filter->num_hash_funcs;
      
      // offset += num_blocks_seeds;

      // // end block data
      // int num_blocks_data = (BLOCK_SIZE + data_size - 1) / data_size;
      // metadata_block[4] = bloom_filter->num_hash_funcs;
      
      // // num data blocks
      // metadata_block[5] = data.size();

      // std::string to_find = "SST_0.bin";
      // if (options.filepath.find(to_find)) {

      //   std::cout << bit_array.size() << std::endl;
      // }
      

      // // Write metadata block
      // long *metadata_block_buffer = reinterpret_cast<long*>(metadata_block.data());
      // int code = file_utils::write_data(options.filepath, metadata_block_buffer, BLOCK_SIZE);
      // if(code == -1) {
      //   perror("Error writing to file");
      //   return -1;
      // }
 
      // // Write Bloom filter bit values
      
      // bit_array.resize(num_blocks_bits * BLOCK_SIZE, 0);
      // long *bit_array_buffer = reinterpret_cast<long*>(bit_array.data());
      // code = file_utils::write_data(options.filepath, bit_array_buffer, bit_array_size);
      // if(code == -1) {
      //   perror("Error writing to file");
      //   return -1;
      // }



      // // Write Bloom filter Seed values
      // long *seed_array_buffer = reinterpret_cast<long*>(bloom_filter->seed_values.data());
      // code = file_utils::write_data(options.filepath, seed_array_buffer, seeds_array_size);
      // if(code == -1) {
      //   perror("Error writing to file");
      //   return -1;
      // }

      // std::string to_find = "SST_0.bin";
      // if (options.filepath.find(to_find)) {

      //   std::cout << bit_array.size() << std::endl;
      // }
      

    }

    // Write the data
    code = file_utils::write_data(options.filepath, data_buffer, data_size);
    if(code == -1) {
      perror("Error writing to file");
      return -1;
    }
  }
}

long ListSST::get_num_blocks() {
  long filesize = file_utils::get_filesize(options.filepath);
  if(filesize == 0) {
    return -1;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }
  return num_blocks;
}

long ListSST::get_num_entries() {
  long filesize = file_utils::get_filesize(options.filepath);
  if(filesize == 0) {
    return 0;
  }
  return filesize / (KEY_SIZE + VALUE_SIZE);
}

int ListSST::read_block(void * &buffer, long block_num){
  if (block_num >= get_num_blocks()) {
    return 0;
  }
  int bytes_read;
  if(options.use_buffer_pool) {
    bytes_read = file_utils::read_block_buffer_pool(options.buffer_pool, buffer, options.filepath, block_num);
  } else {
    bytes_read = file_utils::read_block(buffer, options.filepath, block_num);
  }

  return bytes_read / (KEY_SIZE + VALUE_SIZE);
}

long ListSST::search(long key, bool &val_found) {
  val_found = false;

  if(options.use_bloom_filter) {
    if(bloom_filter_loaded) {
      bool bloom_contains_key = bloom_filter->contains_key(key);
      if(!bloom_contains_key) {
        return -1;
      }
    } else {
      // TODO: Go through entire SST file and load in bloom filter
    }
  }

  long num_blocks = get_num_blocks();
  
  long block_containing_key = search_utils::binary_search_blocks(options.filepath, num_blocks, key);

  // Binary search on the key-value pairs within the block 
  long found_value = -1;

  std::vector<std::pair<long, long> > key_values;

  if(block_containing_key >= 0) {
    void *buffer;
    int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
    if(error_code != 0) {
      perror("Could not allocate buffer");
      return -1;
    }
    int bytes_read;
    if(options.use_buffer_pool) {
      // Check buffer pool for block
      bytes_read = file_utils::read_block_buffer_pool(options.buffer_pool, buffer, options.filepath, block_containing_key);
    } else {
      bytes_read = file_utils::read_block(buffer, options.filepath, block_containing_key);
    }
    
    key_values = file_utils::reintrepret_buffer(buffer, bytes_read);
    long found_index = search_utils::binary_search_kv(key_values, key);
    
    if (found_index >= 0) {
      val_found = true;
      found_value = key_values[found_index].second;
      free(buffer);
      return found_value;
    } 
    free(buffer);
  }

  return found_value;
}


int ListSST::scan(std::vector<std::pair<long, long> > &result, long key1, long key2) {
  int scan_result_size = 0;

  long num_blocks = get_num_blocks();

  
  // Binary search to find first block containing range
  long block_containing_key = search_utils::binary_search_range_blocks(options.filepath, num_blocks, key1, key2);
  
  if(block_containing_key >= 0) {

    void *buffer;
    int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
    if(error_code != 0) {
      perror("Could not allocate buffer");
      return -1;
    }
    int bytes_read;

    bytes_read = file_utils::read_block(buffer, options.filepath, block_containing_key);

    std::vector<std::pair<long, long> > key_values = file_utils::reintrepret_buffer(buffer, bytes_read);

    // Search every pair in the blocks for the range (starting from the first block that contains the range)
    long block_containing_range = block_containing_key;
    while(block_containing_range < num_blocks) {
      bytes_read = file_utils::read_block(buffer, options.filepath, block_containing_range);
      key_values = file_utils::reintrepret_buffer(buffer, bytes_read);
      for(int i = 0; i < key_values.size(); i++) {
        if((key_values[i].first <= key2)) {
          if(key_values[i].first >= key1) {
            result.push_back(key_values[i]);
            scan_result_size++;
          }
        } else {
          free(buffer);
          // Since file is sorted, must terminate at first key that is greater than the range  
          return scan_result_size;
        }
      }
      block_containing_range++;
    }
    free(buffer);
  }
  return scan_result_size;
}

int ListSST::delete_sst_file() {

  int code = std::remove(options.filepath.c_str());

  if(code != 0){
    perror("Could not delete file");
    return -1;
  }
  return 0;
}