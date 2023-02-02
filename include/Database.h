#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <tuple>
#include "AVL.h"

class Database {
  private:
    AVL_Tree memtable;
  public:
    Database (int memtable_size);
    int memtable_size;
    void open();
    void put(int key, int value);
    int get(int key);
    std::vector<int> scan(int key1, int key2);
    void close(); 
};

#endif