#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <list>
#include "AVL.h"
#include "Storage.h"

class Database {
  private:
    
  public: 
    AVL_Tree *memtable;
    Storage *storage;
    Database (int memtable_size);
    int memtable_size;
    void open();
    void put(int key, int value);
    int get(int key);
    std::list<std::pair<long int, long int>> scan(int key1, int key2);
    void close(); 
};

#endif