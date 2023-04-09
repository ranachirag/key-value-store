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

StorageAppendOnly::StorageAppendOnly(StorageOptions options) : options(options) {
  std::vector<SST *> SSTs;
}


StorageAppendOnly::StorageAppendOnly(StorageOptions options, std::vector<std::string> sst_files) : options(options) {
  std::vector<SST *> SSTs;

  for (std::string filename : sst_files) {
    create_SST(filename, true);
  }
}

int StorageAppendOnly::num_SST() {
  return SSTs.size();
}

std::string StorageAppendOnly::get_SST_filepath(std::string sst_filename) {
  std::string filepath = options.db_name + "/" + sst_filename;
  return filepath;
}

std::string StorageAppendOnly::get_SST_filename() {
  int num_SST_total = num_SST();
  std::string filename = "SST_" + std::to_string(num_SST_total) + ".bin";
  return filename;
}

SST *StorageAppendOnly::create_SST(std::string filename, bool file_exists) {
  std::string filepath = get_SST_filepath(filename);
  SST *sst_to_add;

  SSTOptions sst_options;
  sst_options.filepath = filepath;
  sst_options.use_bloom_filter = options.use_bloom_filters;
  sst_options.bloom_filter_options = options.bloom_filter_options;
  sst_options.file_exists = file_exists;
  sst_options.sst_capacity = options.memtable_capacity;
  sst_options.use_buffer_pool = options.use_buffer_pool;
  sst_options.buffer_pool = options.buffer_pool;

  if(options.sst_structure == LIST_SST) {
    sst_to_add = new ListSST(sst_options);
  } 

  SSTs.emplace_back(sst_to_add);
  
  return sst_to_add;
}

void StorageAppendOnly::add_to_storage(std::vector<std::pair<long, long> > &data) {
  std::string filename = get_SST_filename();  
  SST *added_sst = create_SST(filename, false);
  added_sst->write_to_file(data);
}

long StorageAppendOnly::get_value(long key, bool &val_found) {
  long val;

  for (auto it = SSTs.rbegin(); it != SSTs.rend(); ++it){
    SST *sst = *it;
    val = sst->search(key, val_found);
    if (val_found) {
      return val;
    }
  }
  val_found = false;
  return -1;
}

int StorageAppendOnly::scan_storage(std::vector<std::pair<long, long> > &result,long key1, long key2) {
  int scan_size;
  for (auto it = SSTs.rbegin(); it != SSTs.rend(); ++it){
    SST *sst = *it;
    scan_size += sst->scan(result, key1, key2);
  }

  return scan_size;
}

void StorageAppendOnly::reset() {
  for (auto ptr : SSTs) {
    delete ptr;
  }
  SSTs.clear();
}