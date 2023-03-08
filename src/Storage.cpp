#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "Storage.h"
#include "SST.h"

Storage::Storage(std::string db_name) : db_name(db_name) {
  std::vector<SST *> SSTs;
}


Storage::Storage(std::string db_name, std::vector<std::string> sst_files) : db_name(db_name) {
  std::vector<SST *> SSTs;

  for (std::string filename : sst_files) {
    create_SST(filename);
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

void Storage::create_SST(std::string filename) {
  std::string filepath = get_SST_filepath(filename);
  SST *sst_to_add = new SST(filepath);
  SSTs.push_back(sst_to_add);
}

int Storage::add_to_storage(std::vector<std::pair<long, long>> data) {
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

  create_SST(filename);
}

long Storage::get_value(long key, bool &val_found) {
  long val;
  for(SST *sst : SSTs) {
    val = sst->search(key, val_found);
    if (val_found) {
      return val;
    }
  } 
  val_found = false;
  return -1;
}

