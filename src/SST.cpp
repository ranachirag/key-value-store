#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string>
#include <utility>

#include <sys/stat.h>

#include "SST.h"
#include "utils.h"


ListSST::ListSST(std::string sst_filepath) {
  filepath = sst_filepath;
}

long ListSST::search(bool use_buffer_pool, BufferPool *buffer_pool, long key, bool &val_found) {
  val_found = false;

  long filesize = file_utils::get_filesize(filepath);
  if(filesize == 0) {
    return -1;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }
  
  long block_containing_key = search_utils::binary_search_blocks(filepath, num_blocks, key);

  // Binary search on the key-value pairs within the block 
  long found_value = -1;

  std::vector<std::pair<long, long> > key_values;

  void *buffer;
  int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
  if(error_code != 0) {
    perror("Could not allocate buffer");
    return -1;
  }
  int bytes_read;

  if(block_containing_key >= 0) {
    if(use_buffer_pool) {
      bytes_read = file_utils::read_block_buffer_pool(buffer_pool, buffer, filepath, block_containing_key);
    } else {
      bytes_read = file_utils::read_block(buffer, filepath, block_containing_key);
    }
    key_values = file_utils::reintrepret_buffer(buffer, bytes_read);
    long found_index = search_utils::binary_search_kv(key_values, key);

    if (found_index >= 0) {
      val_found = true;
      found_value = key_values[found_index].second;

      if(!use_buffer_pool) {
        free(buffer);
      }
      return found_value;
    } 
  }
  if(!use_buffer_pool) {
    free(buffer);
  }
  return found_value;
}


int ListSST::scan(std::vector<std::pair<long, long> > &result, long key1, long key2) {
  int scan_result_size = 0;
  long filesize = file_utils::get_filesize(filepath);
  if(filesize == 0) {
    return scan_result_size;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }
  
  // Binary search to find first block containing range
  long block_containing_key = search_utils::binary_search_range_blocks(filepath, num_blocks, key1, key2);
  
  if(block_containing_key >= 0) {

    void *buffer;
    int error_code = posix_memalign(&buffer, BLOCK_SIZE, BLOCK_SIZE);
    if(error_code != 0) {
      perror("Could not allocate buffer");
      return -1;
    }
    int bytes_read;

    bytes_read = file_utils::read_block(buffer, filepath, block_containing_key);

    std::vector<std::pair<long, long> > key_values = file_utils::reintrepret_buffer(buffer, bytes_read);

    // Search every pair in the blocks for the range (starting from the first block that contains the range)
    long block_containing_range = block_containing_key;
    while(block_containing_range < num_blocks) {
      bytes_read = file_utils::read_block(buffer, filepath, block_containing_range);
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