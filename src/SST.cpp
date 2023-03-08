#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <utility>
#include "SST.h"

// TODO: Move to a utils file????
std::vector<std::pair<long, long>> read_block(std::string filepath, long block_num) {
  int fd = open(filepath.c_str(), O_RDONLY | O_CREAT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  long offset = block_num * BLOCK_SIZE;

  char buffer[BLOCK_SIZE];

  long bytes_read = pread(fd, buffer, BLOCK_SIZE, offset);

  std::vector<std::pair<long, long>> values (reinterpret_cast<std::pair<long, long>*>(buffer), 
                                             reinterpret_cast<std::pair<long, long>*>(buffer + bytes_read));

  return values;

}

// TODO: Move to a utils file????
long get_filesize(std::string filepath) {
  struct stat file_info;
  if (stat(filepath.c_str(), &file_info) == -1) {
    perror("Error reading file info");
    return -1;
  }
  long filesize = file_info.st_size;
  return filesize;
}


long binary_search_blocks(std::string filepath, long num_blocks, long key) {

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  long min_key_block;
  long max_key_block;

  std::vector<std::pair<long, long>> values;

  // Binary search over
  while (start_block <= end_block) {
    block_to_read = start_block + ((end_block - start_block) / 2);
    values = read_block(filepath, block_to_read);
    min_key_block = values[0].first;
    max_key_block = values.back().first;
    if (key < min_key_block ) {
      end_block = block_to_read - 1;
    } else if (key > max_key_block) {
      start_block = block_to_read + 1;
    } else {
      return block_to_read;
    }
  }

  // TODO: free std::vector<std::pair<long, long>> values ?

  return -1;
}

// Returns index in key_values where matching key is found
long binary_search_kv(std::vector<std::pair<long, long>> key_values, long key) {
  long mid_index;
  long mid_key;
  long start_index = 0;
  long end_index = key_values.size()-1;

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


long SST::search(long key, bool &val_found) {
  val_found = false;

  long filesize = get_filesize(filepath);
  if(filesize == 0) {
    return -1;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }
  
  long block_containing_key = binary_search_blocks(filepath, num_blocks, key);

  // Binary search on the key-value pairs within the block 
  long found_value = -1;
  if(block_containing_key >= 0) {
    std::vector<std::pair<long, long>> key_values = read_block(filepath, block_containing_key);

    long found_index = binary_search_kv(key_values, key);

    if (found_index >= 0) {
      val_found = true;
      found_value = key_values[found_index].second;
      return found_value;
    } 
  }

  // TODO: free std::vector<std::pair<long, long>> key_values ?
   
  return found_value;
}


std::vector<std::pair<long, long>> SST::scan(long key1, long key2) {
  std::vector<std::pair<long, long>> kv_found {};
  long filesize = get_filesize(filepath);
  if(filesize == 0) {
    return kv_found;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }
  
  long start_key = key1;
  long end_key = key2;


  
  long block_containing_key = binary_search_blocks(filepath, num_blocks, key1);
  
  if(block_containing_key >= 0) {

    std::vector<std::pair<long, long>> key_values = read_block(filepath, block_containing_key);

    long found_index = binary_search_kv(key_values, key1);
    if (found_index >= 0) {
      
      // TODO: Fix repetitive code

      for(int i = found_index; i < key_values.size(); i++) {
        if(key_values[i].first <= key2) {
          kv_found.push_back(key_values[i]);
        } else {
          return kv_found;
        }
      }

      long block_containing_range = block_containing_key + 1;
      while(block_containing_range < num_blocks) {
        key_values = read_block(filepath, block_containing_range);
        for(int i = 0; i < key_values.size(); i++) {
          if(key_values[i].first <= key2) {
            kv_found.push_back(key_values[i]);
          } else {
            return kv_found;
          }
        }
      }
      
    } 
  }
  return kv_found;
}