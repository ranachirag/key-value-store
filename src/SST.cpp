#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <utility>
#include "SST.h"


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


long SST::search(long key, bool &val_found) {
  val_found = false;
  struct stat file_info;
  if (stat(filepath.c_str(), &file_info) == -1) {
    perror("Error reading file info");
    return -1;
  }
  long filesize = file_info.st_size;
  if(filesize == 0) {
    return -1;
  }
  long num_blocks = filesize / BLOCK_SIZE;
  if ((num_blocks * BLOCK_SIZE) < filesize) {
    num_blocks++;
  }

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  bool val_block_found = false;
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
      val_block_found = true;
      break;
    }
  }

  // Binary search on the key-value pairs within the block 
  long potential_value = -1;
  if(val_block_found) {
    long mid_index;
    long mid_key;
    long start_index = 0;
    long end_index = values.size()-1;

    while (start_block <= end_block) {
      mid_index = start_index + ((end_index - start_index) / 2);
      mid_key = values[mid_index].first;
      potential_value = values[mid_index].second;

      if (key < mid_key) {
        end_index = mid_index - 1; 
      } else if (key > mid_key) {
        start_index = mid_index + 1;
      } else {
        val_found = true;
        return potential_value;
      }
    }
  }

  // TODO: free std::vector<std::pair<long, long>> values;
   
  return potential_value;
}