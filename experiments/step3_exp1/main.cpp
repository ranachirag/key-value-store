#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include "BufferPool.h"
#include <chrono>
#include <iomanip>
#include "Step3Experiment1.h"
#include "DatabaseMacros.h"
#include <fstream>

int main() {

  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB Buffer Pool size limit
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5; // 5 bits per entry for the Bloom Filter

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1; // 1 MB 
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;

  int arr_size = 8;
  std::cout << "mb_inserted,put_time,get_time,scan_time" << std::endl;
  Step3Experiment1 *exp = new Step3Experiment1(1024, db_options);
  exp->run_experiments(1024, 64);


}