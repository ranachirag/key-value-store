#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include "BufferPool.h"
#include "DatabaseMacros.h"

int main() {
  // Memtable memtable = new Memtable(5);
  // std::string db_name = "test1";
  // Database *kv_store = new Database(8*100);
  // kv_store->open(db_name);

  // int range = 1000;
  // for(int i = 0; i < range; i++) {
  //   // std::cout << "Iteration: " << i << std::endl;
  //   kv_store->put(i, 1000-i);
  // }

  // // long val = kv_store->get(578);
  // std::vector<std::pair<long, long> > result;
  // int result_size = kv_store->scan(result, 750, 950);
  // std::cout << "HERE: " << result_size << std::endl;
  // for (std::pair<long, long> pair : result) {
  //     std::cout << pair.first << " " << pair.second << std::endl;
  // }
  // kv_store->close();


  // std::string db_name2 = "test2";
  // kv_store->open(db_name2);

  // int range2 = 10000;
  // for(int i = 0; i < range2; i++) {
  //   // std::cout << "Iteration: " << i << std::endl;
  //   kv_store->put(i, 10000-i);
  // }

  // long val = kv_store->get(578);
  // std::vector<std::pair<long, long> > result2;
  // result_size = kv_store->scan(result2, 750, 9500);
  // for (std::pair<long, long> pair : result2) {
  //     std::cout << pair.first << " " << pair.second << std::endl;
  // }
  // kv_store->close();
  // avl_tree->print_tree();
  // std::vector<std::pair<long, long>> lst = avl_tree->range_search(0, range);
  // std::cout << "DONE RANGE SEARCH" << std::endl;
  // std::for_each(lst.begin(),
  //               lst.end(),
  //               [](const auto &e) {
  //                   std::cout << "Key: " << e.first << " Value: " << e.second << std::endl;
  //               });

  // std::string hash_key = "test";
  // int hash_val = hash_utils::get_hash_value(hash_key, 443);
  // std::cout << hash_val << std::endl;

  
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


  std::string db_name = "step1_experiment_1MB";
  kv_store->open(db_name);
  // start measuring time here
  // for (int i = 0; i < 1; ++i) {
  //   for(int j = 0; j < 131072; ++j) {
  //     int key_val = i*131072 + j;
  //     kv_store->put(key_val, key_val);
  //   }
  // }

  for (int i = 0; i < 1000000; ++i) {
      
      if(i == 700000) {
        kv_store->update_bloom_filter_param(8); // TODO: Remove not working 
      }
      // int key_val = i;
      // kv_store->put(key_val, key_val+1);
      kv_store->put(i, i);
      // std::cout << "HERE: " << kv_store->get(i) << std::endl;
  }

  
  std::cout << "HERE: " << kv_store->get(1000) << std::endl;
  

  // std::vector<std::pair<long, long> > vals;
  // int entries = kv_store->scan(vals, 1000000-3, 1000010);
  // std::cout << entries << std::endl;

  // for(std::pair<long, long> pair : vals) {
  //   std::cout << pair.first << ": " << pair.second << std::endl;
  // }


  // for (int i = -1000; i < 1001; ++i) {
  //   int key_val = i;
  //   long val = kv_store->get(key_val);
  //   std::cout << key_val << ": " << val << std::endl;
  // }

  // long val = kv_store->get(500);
  // std::cout << "BEFORE DELETE" << ": " << val << std::endl;

 
  // std::cout << "AFTER DELETE" << ": " << val << std::endl;



  // for (int i = 0; i < 1; ++i) {
  //   for(int j = 131070; j < 131072; ++j) {
  //     int key_val = i*131072 + j;
  //     long val = kv_store->get(key_val);
  //     std::cout << key_val << ": " << val << std::endl;
  //   }
  // }

  // std::vector<std::pair<long, long> > vals;
  // int entries = kv_store->scan(vals, 1, 1000);
  // std::cout << vals[999].first << std::endl;
  kv_store->close();

  return 0; 

}