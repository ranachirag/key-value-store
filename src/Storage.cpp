#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <string>
#include "Storage.h"
#include "SST.h"

Storage::Storage(std::string db_name) : db_name(db_name) {
  // TODO: Initialize to the SSTs to empty vector
  std::vector<SST *> SSTs;
}


Storage::Storage(std::string db_name, std::vector<std::string> sst_files) : db_name(db_name) {
  std::vector<SST *> SSTs;

  // TODO: Initialize to the SSTs class array to create SST objects from parameters (filepath of an SST = db_name + / + SST file name)
  for (std::string filename : sst_files) {
    add_SST(filename);
  }
}

int Storage::num_SST() {
  return SSTs.size();
}

std::string Storage::get_SST_filepath(std::string sst_filename) {
  std::string filepath = db_name + "/" + sst_filename;
  return filepath;
}

std::string Storage::get_SST_filename() {
  int num_SST_total = num_SST();
  std::string filename = "SST_" + std::to_string(num_SST_total) + ".bin";
  return filename;
}

void Storage::add_SST(std::string sst_filename) {
  std::string filepath = get_SST_filepath(sst_filename);
  SST *sst_to_add = new SST(filepath);
  SSTs.push_back(sst_to_add);
}

int Storage::create_SST(std::vector<std::pair<long, long>> data) {
  std::string filename = get_SST_filename();
  std::string filepath = get_SST_filepath(filename);
  
  int fd = open(filepath.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  // TODO: Do size, num_blocks, offset, size_unwritten need to be long?

  long* buffer = reinterpret_cast<long*>(data.data());
  long size = data.size() * 2 * sizeof(long);
  long num_blocks = size / BLOCK_SIZE; 

  long offset = 0;
  long size_unwritten = size;
  long* buf_unwritten = buffer;
  const int block_size_long = BLOCK_SIZE / sizeof(long);

  for (long i = 0; i < num_blocks; i++) {
    pwrite(fd, buf_unwritten, BLOCK_SIZE, offset); // Equivalent to 1 I/O
    offset += BLOCK_SIZE;
    size_unwritten -= BLOCK_SIZE;
    buf_unwritten += block_size_long;
  }

  if(size_unwritten > 0) {
    pwrite(fd, buf_unwritten, size_unwritten, offset);
  }

  close(fd);

  // TODO: Add file name to list of SSTs in this storage class
  add_SST(filename);
}

SST Storage::read_SST() {
  char filename[] = "test.bin";
  
  int fd = open(filename, O_RDONLY | O_CREAT); // Add O_DIRECT Flag

  if (fd ==-1) {
      perror("Error opening file");
  }

  // TODO: Read File
  

  close(fd);


}


// Search:
// Loop over all SST files
// call SST.search


