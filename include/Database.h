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
    AVL_Tree *memtable; // "Main Memory"
    Storage *storage; // "Disk Storage"
  public: 
    Database (long memtable_size); 
    long memtable_size; // Size of our main memory (AVL) before flushing to disk (SST)
    void open(std::string db_name); // Read in SST files, AVL tree is always empty
    void put(long key, long value); // Insert into main memory (AVL), if overflows memtable_size, flush into disk
    long get(long key); // Search AVL tree (tree traversal) and all SSTs (binary search) for key
    std::vector<std::pair<long, long>> scan(long key1, long key2); 
    void close(); // Flush current AVL tree (even if not full) as a SST
};

#endif