#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <vector>
#include <string>

#include "AVL.h"
#include "Storage.h"

/**
 * This class represents a Key Value store database
 */
class Database {
  public: 
    /**
     * Constructor
     * @param memtable_size The maximum size of the memtable
    */
    Database (long memtable_size); 

    /**
     * Open the database for usage
     * @param db_name Name of database to open
    */
    void open(std::string db_name);
    
    /**
     * Store a key with an associated value in the database 
     * @param key Key
     * @param value Value
    */
    void put(long key, long value); 

    /**
     * Retrieve the value with for an associated key
     * @param key Key
     * 
     * @return Value associated with given key
    */
    long get(long key); 

    /**
     * Retrieve all key-value pairs in a given range of keys, in sorted order by keys
     * 
     * @param result Results of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs in given range
    */
    int scan(std::vector<std::pair<long, long> > &result, long key1, long key2); 

    /**
     * Close the database
    */
    void close(); 

  private:
    /**
     * The maximum size of the memtable
    */
    const long memtable_size;

    /**
     * A boolean value indicating whether a database is open or not
    */
    bool db_open;

    /**
     * The name of the database currently open
    */
    std::string name;

    /**
     * Memtable
    */
    AVL_Tree * memtable; 

    /**
     * The maximum size of the memtable
    */
    Storage * storage; 
};

#endif