#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <utility>

#include "Storage.h"
#include "SST.h"
#include "utils.h"

Storage::Storage(StorageOptions options) : options(options) {
  std::vector<SST *> SSTs;
}


Storage::Storage(StorageOptions options, std::vector<std::string> sst_files) : options(options) {
  std::vector<SST *> SSTs;

  for (std::string filename : sst_files) {
    create_SST(filename);
  }
}

int Storage::num_SST() {
  return SSTs.size();
}

std::string Storage::get_SST_filepath(std::string sst_filename) {
  std::string filepath = options.db_name + "/" + sst_filename;
  return filepath;
}

std::string Storage::get_SST_filename() {
  int num_SST_total = num_SST();
  std::string filename = "SST_" + std::to_string(num_SST_total) + ".bin";
  return filename;
}

void Storage::create_SST(std::string filename) {
  std::string filepath = get_SST_filepath(filename);
  SST *sst_to_add;
  if(options.sst_structure == LIST_SST) {
    sst_to_add = new ListSST(filepath);
  } 
  // TODO: Potentiall add more options
  SSTs.emplace_back(sst_to_add);
}

void Storage::add_to_storage(std::vector<std::pair<long, long> > &data) {
  std::string filename = get_SST_filename();
  std::string filepath = get_SST_filepath(filename);
  

  long* buffer = reinterpret_cast<long*>(data.data());
  long size = data.size() * 2 * sizeof(long);
  int code = file_utils::write_data(filepath, buffer, size);
  if(code == -1) {
    perror("Error writing to file");
  }

  create_SST(filename);
}

long Storage::get_value(long key, bool &val_found) {
  long val;

  for (auto it = SSTs.rbegin(); it != SSTs.rend(); ++it){
    SST *sst = *it;
    val = sst->search(options.use_buffer_pool, options.buffer_pool, key, val_found);
    if (val_found) {
      return val;
    }
  }
  val_found = false;
  return -1;
}

int Storage::scan_storage(std::vector<std::pair<long, long> > &result,long key1, long key2) {
  int scan_size;
  for (auto it = SSTs.rbegin(); it != SSTs.rend(); ++it){
    SST *sst = *it;
    scan_size += sst->scan(result, key1, key2);
  }

  return scan_size;
}

void Storage::reset() {
  for (auto ptr : SSTs) {
    delete ptr;
  }
  SSTs.clear();
}

