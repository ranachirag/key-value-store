#include <algorithm>
#include <list>
#include "Database.h"
#include "AVL.h"

Database::Database (int memtable_size) : memtable_size(memtable_size) {
  // TODO
  memtable = new AVL_Tree();
  storage = new Storage();
}

void Database::open() {
  // TODO
  // Read directory
}

void Database::put(int key, int value) {
  // TODO

  // If sizeof(key) + memtable->size  > memtable_size, 
  //   call range_scan
  //   create SST
  //   clear memtable (AVL Tree)
  // Insert key, value into memtable
  
}

int Database::get(int key) {
  // TODO

  
}

std::list<std::pair<long int, long int>> Database::scan(int key1, int key2) {
  // TODO
}

void Database::close() {
  // TODO
}