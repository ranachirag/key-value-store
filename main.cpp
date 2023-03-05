#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <list>

int main() {
  // Memtable memtable = new Memtable(5);
  Database *kv_store = new Database(10);

  AVL_Tree *avl_tree = kv_store->memtable;
  avl_tree->insert(5, 101);
  avl_tree->insert(8, 102);
  avl_tree->insert(1, 103);
  avl_tree->insert(100, 104);
  avl_tree->insert(17, 105);
  avl_tree->insert(101, 106);
  avl_tree->insert(78, 107);
  avl_tree->insert(6, 108);
  avl_tree->insert(1000, 109);
  avl_tree->insert(1001, 16);
  avl_tree->insert(1002, 110);
  avl_tree->print_tree();
  std::list<std::pair<long int, long int>> lst = avl_tree->range_search(1, 78);
  std::for_each(lst.begin(),
                lst.end(),
                [](const auto &e) {
                    std::cout << "Key: " << e.first << " Value: " << e.second << std::endl;
                });
  kv_store->storage->create_SST(lst);
}