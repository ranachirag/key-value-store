#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"

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

  std::string hash_key = "test";
  int hash_val = hash_utils::get_hash_value(hash_key);
  std::cout << hash_val << std::endl;
}