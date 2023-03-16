#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string>
#include <utility>

#include <sys/stat.h>
#include "xxhash.h"

#include "utils.h"
#include "SST.h"


long file_utils::get_filesize(std::string filepath) {
  struct stat file_info;
  if (stat(filepath.c_str(), &file_info) == -1) {
    perror("Error reading file info");
    return -1;
  }
  long filesize = file_info.st_size;
  return filesize;
}


std::vector<std::pair<long, long> > file_utils::read_block(std::string filepath, long block_num) {
  int fd = open(filepath.c_str(), O_RDONLY | O_CREAT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  long offset = block_num * BLOCK_SIZE;
  char buffer[BLOCK_SIZE];
  long bytes_read = pread(fd, buffer, BLOCK_SIZE, offset);

  std::vector<std::pair<long, long> > values (reinterpret_cast<std::pair<long, long>*>(buffer), 
                                             reinterpret_cast<std::pair<long, long>*>(buffer + bytes_read));

  return values;

}

int file_utils::write_data(std::string filepath, long* data, long data_size) {
  int fd = open(filepath.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666); // Add O_DIRECT Flag

  if (fd == -1) {
      perror("Error opening file");
      return -1;
  }

  long num_blocks = data_size / BLOCK_SIZE; 

  long offset = 0;
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


long search_utils::binary_search_blocks(std::string filepath, long num_blocks, long key) {

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  long min_key_block;
  long max_key_block;

  std::vector<std::pair<long, long> > values;

  while (start_block <= end_block) {
    block_to_read = start_block + ((end_block - start_block) / 2);
    values = file_utils::read_block(filepath, block_to_read);
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

  return -1;
}

long search_utils::binary_search_range_blocks(std::string filepath, long num_blocks, long key1, long key2) {

  long start_block = 0;
  long end_block = num_blocks - 1;
  long block_to_read;

  long min_key_block;
  long max_key_block;

  std::vector<std::pair<long, long> > values;

  while (start_block <= end_block) {
    block_to_read = start_block + ((end_block - start_block) / 2);
    values = file_utils::read_block(filepath, block_to_read);
    min_key_block = values[0].first;
    max_key_block = values.back().first;
    if (key2 < min_key_block) {
      end_block = block_to_read - 1;
    } else if (key1 > max_key_block) {
      start_block = block_to_read + 1;
    } else {
      return block_to_read;
    }
  }

  return -1;
}

long search_utils::binary_search_kv(std::vector<std::pair<long, long> > key_values, long key) {
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


int hash_utils::get_hash_value(std::string hash_key) {
  int seed = 443;

  long hash = XXH64(hash_key.c_str(), hash_key.length(), seed);

  std::cout << "hash = " << hash << std::endl;
  return 0;
}