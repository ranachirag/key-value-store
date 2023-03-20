#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include "BufferPool.h"

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

  
  
  BufferPool *buf_pool = new BufferPool(4, 128);
  // std::string* test_data = new std::string("Hello, World!");
  // buf_pool->insert_data(hash_key, (void *)test_data);

  // void* data = nullptr;
  // int found = buf_pool->get_data(data, hash_key);

  // std::string *found_val = (std::string *) data;
  // std::cout << found << std::endl;
  // std::cout << *found_val << std::endl;

  for(int i = 0; i < 50; ++i) {
    std::string hash_key = "hash_key" + std::to_string(i);
    std::string* test_data = new std::string("Hello, World!" + std::to_string(i));
    buf_pool->insert_data(hash_key, (void *)test_data);
  }
  buf_pool->print_bufpool();

  for(int i = 0; i < 50; ++i) {
    void* data = nullptr;
    std::string hash_key = "hash_key" + std::to_string(i);
    int found = buf_pool->get_data(data, hash_key);

    std::string *found_val = (std::string *) data;
    // std::cout << found << std::endl;
    std::cout << *found_val << std::endl;
  }
  buf_pool->print_bufpool();



}