#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <vector>
#include <string>
#include "AVL.h"
#include "Storage.h"

class Database {
  private:
    std::string name;
    bool db_open;
  public: 
    AVL_Tree *memtable;
    Storage *storage;
    Database (long memtable_size);
    long memtable_size;
    void open(std::string db_name);
    void put(long key, long value);
    long get(long key);
    std::vector<std::pair<long, long>> scan(long key1, long key2);
    void close(); 
};

#endif