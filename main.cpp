#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>

int main() {
  // Memtable memtable = new Memtable(5);
  std::string db_name = "test1";
  Database *kv_store = new Database(10);

  AVL_Tree *avl_tree = kv_store->memtable;
  int range = 10000;
  for(int i = 0; i < range; i++) {
    avl_tree->insert(i, i);
  }
  // avl_tree->print_tree();
  std::vector<std::pair<long, long>> lst = avl_tree->range_search(0, range);
  std::cout << "DONE RANGE SEARCH" << std::endl;
  // std::for_each(lst.begin(),
  //               lst.end(),
  //               [](const auto &e) {
  //                   std::cout << "Key: " << e.first << " Value: " << e.second << std::endl;
  //               });
  kv_store->storage->create_SST(lst);
}