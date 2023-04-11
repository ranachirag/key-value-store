#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "Database.h"
#include "DatabaseMacros.h"

int main() {


  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5;

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1;
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;


  Database *kv_store = new Database(db_options);


  std::string db_name = "kv_store_1";
  kv_store->open(db_name);

  for (int i = 0; i < 1000000; ++i) {
    kv_store->put(i, i);
  }
  
  kv_store->close();

  return 0; 

}