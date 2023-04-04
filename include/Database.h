#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <vector>
#include <string>

#include "AVL.h"
#include "Storage.h"
#include "BufferPool.h"

/**
 * This represents the options to configure the Database
 */
struct DatabaseOptions {
  /**
   * The maximum size of the memtable
  */
  long memtable_size;

  /**
   * Use a Buffer Pool for storage
   */
  bool use_buffer_pool = true;

  /**
   * Buffer Pool configurations
   */
  BufferPoolOptions buffer_pool_options;

  /**
   * Structure of SST
   */
  std::string sst_structure = "LIST"; // TODO: REMOVE ?
};

/**
 * This class represents a Key Value store database
 */
class Database {
  public: 
    /**
     * Constructor
     * @param memtable_size The maximum size of the memtable
    */
    Database (DatabaseOptions options); 

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
     * Update maximum size of the buffer pool, if configured.
     * 
     * @param new_max_size Updated maximum size of the buffer pool (in units of 4 KB)
     * @return 0 if successfully updated, -1 otherwise
     */
    int update_buffer_pool_size(int new_max_size);

    /**
     * Close the database
    */
    void close(); 

  private:

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

    /**
     * Database configurations
     */
    DatabaseOptions options;

    /**
     * Buffer Pool
     */
    BufferPool *buffer_pool;
};

#endif