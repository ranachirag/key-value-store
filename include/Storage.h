#ifndef STORAGE_H
#define STORAGE_H

#include <algorithm>
#include <vector>
#include <string>

#include "SST.h"
#include "BufferPool.h"
#include "BloomFilter.h"

/**
 * This struct represents the configuration options for a Storage object
 */
struct StorageOptions {
  /**
   * Name of the database the storage is for
   */
  std::string db_name;

  /**
   * True if the Buffer Pool is to be used, False otherwise
   */
  bool use_buffer_pool;

  /**
   * Buffer Pool object (if the Buffer Pool is to be used)
   */
  BufferPool *buffer_pool;

  /**
   * The structure of SST to use 
   * 
   * Note: Currently only the List structure is supported
   */
  std::string sst_structure;

  /**
   * True if the Bloom Filters are to be used for each SST, False otherwise
   */
  bool use_bloom_filters;

  /**
   * Bloom Filter configuration options if the Bloom Filters are to be used 
   */
  BloomFilterOptions bloom_filter_options;

  /**
   * Capacity of the memtable, equivalent to the capacity of the SST file (only the first one in case of LSM Tree)
   */
  int memtable_capacity;
};

/**
 * This class represents an interface for SST storage
*/
class Storage {
  public:
    /**
     * Add a list of key-value pairs to SST storage
     * 
     * @param data Key-value pairs (sorted by keys)
    */
    virtual void add_to_storage(std::vector<std::pair<long, long> > &data) = 0;

    /**
     * Retreive a value with a given associated key from SST storage
     * 
     * @param key Key to search
     * @param val_found Indicates if the value was found
     * 
     * @return Value found from searching SST storage with given key
    */
    virtual long get_value(long key, bool &val_found) = 0;

    /**
     * Retreive all key-value pairs with a given key range from SST storage
     * 
     * @param result Results of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs found within given key range
    */
    virtual int scan_storage(std::vector<std::pair<long, long> > &result, long key1, long key2) = 0;

    /**
     * Reset Storage object
     */
    virtual void reset() = 0;

};


struct LevelLSMOptions {
  int sst_capacity;
  int level_num;
  std::string db_name;
  bool use_bloom_filters;
  BloomFilterOptions bloom_filter_options;
  std::string sst_structure;
  bool use_buffer_pool;
  BufferPool *buffer_pool;
};

/**
 * This class represents an interface for a level of the LSM Tree
 */
class LevelLSM {
  public:
    /**
     * @brief Construct a new Level LSM object
     * 
     * @param options LSM Tree Level configuration options
     */
    LevelLSM(LevelLSMOptions options);

    /**
     * Read in an (existing) SST file for this level or pass it down to next level if it does not match
     * 
     * @param sst_filename Name of the SST file
     * 
     * @return 0 if successful, -1 otherwise
     */
    int initialize_level(std::string sst_filename);

    /**
     * Add an SST object to the level (passed in from level above as a result of compaction)
     * 
     * @param sst SST object
     * 
     * @return 0 if successful, -1 otherwise 
     */
    int add_sst(SST *sst);

    /**
     * Create an SST object given Key-Value pairs and add that SST object to current LSM tree level
     * 
     * @param data Key-Value pairs
     * 
     * @return 0 if successful, -1 otherwise 
     */
    int add_sst(std::vector<std::pair<long, long>> &data);

    /**
     * Returns the number of SST object the LSM tree level holds
     * 
     * @return The number of SST object the LSM tree level holds
     */
    int num_ssts();

    /**
     * Search a value in the current LSM tree level and the levels below (if any exist)
     * 
     * @param key =
     * @param val_found 
     * @return long 
     */
    long get_value(long key, bool &val_found);

  private:
    /**
     * Compact the 
     * 
     * @return int 
     */
    int compact_SSTs();
    std::string level_LSM_filepath(int level_num, int sst_num);
    LevelLSMOptions options;
    std::vector<SST *> SSTs;
    LevelLSM *next_level;
};

/**
 * This class represents an interface for SST storage using LSM tree 
*/
class StorageLSM : public Storage {
  public:
    /**
     * Construct a new Storage LSM object
     * 
     * @param options Storage configuration
     */
    StorageLSM(StorageOptions options);

    /**
     * Construct a new Storage LSM object
     * 
     * @param options Storage configuration
     * @param sst_files SST filenames
     */
    StorageLSM(StorageOptions options, std::vector<std::string> sst_files);

    /**
     * Add a list of key-value pairs to SST storage
     * 
     * @param data Key-value pairs (sorted by keys)
    */
    void add_to_storage(std::vector<std::pair<long, long> > &data);

    /**
     * Retreive a value with a given associated key from SST storage
     * 
     * @param key Key to search
     * @param val_found Indicates if the value was found
     * 
     * @return Value found from searching SST storage with given key
    */
    long get_value(long key, bool &val_found);

    /**
     * Retreive all key-value pairs with a given key range from SST storage
     * 
     * @param result Results of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs found within given key range
    */
    int scan_storage(std::vector<std::pair<long, long> > &result, long key1, long key2);

    /**
     * Reset Storage object
     */
    void reset();

  private:

    /**
     * Linked list representation of the Levels of the LSM Tree
     * First level of the LSM Tree.
     */
    LevelLSM *first_level;

    /**
     * Storage configurations
     */
    StorageOptions options;
};

/**
 * This class represents an interface for SST storage using Append Only structure
*/
class StorageAppendOnly : public Storage {
  public:
    /**
     * Constructor for empty SST storage
     * 
     * @param options Storage configuration
    */
    StorageAppendOnly(StorageOptions options);

    /**
     * Constructor given an intial set of SSTs (sorted by oldest to youngest)
     * 
     * @param options Storage configuration
     * @param sst_files SST filenames
    */
    StorageAppendOnly(StorageOptions options, std::vector<std::string> sst_files);

    /**
     * Add a list of key-value pairs to SST storage
     * 
     * @param data Key-value pairs (sorted by keys)
    */
    void add_to_storage(std::vector<std::pair<long, long> > &data);

    /**
     * Retreive a value with a given associated key from SST storage
     * 
     * @param key Key to search
     * @param val_found Indicates if the value was found
     * 
     * @return Value found from searching SST storage with given key
    */
    long get_value(long key, bool &val_found);

    /**
     * Retreive all key-value pairs with a given key range from SST storage
     * 
     * @param result Results of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs found within given key range
    */
    int scan_storage(std::vector<std::pair<long, long> > &result, long key1, long key2);

    /**
     * Reset Storage object
     */
    void reset();

  private:
    /**
     * Calculates the number of SST files in SST storage
     * 
     * @return Number of SST files in SST storage
    */
    int num_SST();

    /**
     * Get the filename of the next (youngest) SST to insert 
     * 
     * Note: Structure of SST files are: `SST_{NUM}.bin` where NUM is an integer value representing the
     *       age of SST file. For example, if there are 10 existing SST files in storage, the oldest file
     *       would be `SST_0.bin` and the youngest existing file would be `SST_9.bin` and this function 
     *       would return the string value `SST_10.bin` representing the new youngest file to insert into
     *       storage.
     *
     * @return Filename of the youngest SST 
    */
    std::string get_SST_filename(); 

    /**
     * Construct path for a SST file
     * 
     * @param sst_filename SST filename
     * 
     * @return Path to the SST file from project root directory
    */
    std::string get_SST_filepath(std::string sst_filename);

    /**
     * Create a SST object for an existing SST binary file
     * 
     * @param filename Name of SST file to create object for
     * @param file_exists True if a file for SST object to be created exists, False otherwise
     * @return The SST object that was created
    */
    SST *create_SST(std::string filename, bool file_exists);

    /**
     * List of object representations of all the SSTs in storage
    */
    std::vector<SST*> SSTs;

    /**
     * Storage configurations
     */
    StorageOptions options;

};

#endif