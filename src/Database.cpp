#include <algorithm>
#include <list>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "Database.h"
#include "AVL.h"

Database::Database (long memtable_size) : memtable_size(memtable_size){
  db_open = false;
}

void Database::open(std::string db_name) {
  // TODO
  // Read directory
  if (db_open) {
    std::string err_msg = "Please close the " + name + " database first";
    perror(err_msg.c_str());
    return;
  }
  db_open = true;
  name = db_name;

  // TODO: Create a directory with name db_name if it already does not exist

  // TODO: Retreive list of all filenames in the directory (if directory exists)

  // TODO: Sort the files by name (filenames are dbName_SST_0.bin, dbName_SST_1.bin, etc where lower numbers are older and higher numbers are )

  // TODO: Use different Storage constructors based on if SSTs already exist or not

  memtable = new AVL_Tree();
  storage = new Storage(name);
}

void Database::put(long key, long value) {
  // TODO

  // If sizeof(key) + memtable->size  > memtable_size, 
  //   call range_scan
  //   create SST
  //   clear memtable (AVL Tree)
  // Insert key, value into memtable
  
}

long Database::get(long key) {
  // TODO
  
  
}

std::vector<std::pair<long, long>> Database::scan(long key1, long key2) {
  // TODO
}

void Database::close() {
  // TODO
}