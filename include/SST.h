#ifndef SST_H
#define SST_H

#include <string>

#include "BufferPool.h"
#include "DatabaseMacros.h"
#include "BloomFilter.h"

/**
 * This struct represents the possible configuration options for a SST object
 */
struct SSTOptions {
  /**
   * Use Bloom Filter for the SST
   */
  bool use_bloom_filter;

  /**
   * Bloom Filter configurations
   */
  BloomFilterOptions bloom_filter_options;

  /**
   * File path of the SST (for creation or for reading)
   */
  std::string filepath;
  
  /**
   * Number of entries the SST can hold
   */
  int sst_capacity;

  /**
   * True if file exists for SST, False if no file exists
   * 
   * Note: SST files are static, once they are written to fully, they don't change
   */
  bool file_exists;

  /**
   * Use Buffer Pool
   */
  bool use_buffer_pool;
  
  /**
   * Buffer Pool
   */
  BufferPool *buffer_pool;
};

/**
 * This class represents an interface to interact with a SST file
 */
class SST {
  public: 
    /**
     * Destroy the SST object
     */
    virtual ~SST() = default;

    /**
     * Write data to the SST file
     * 
     * @param data Key-Value pairs
     * 
     * @return 0 if successful, -1 otherwise
     */
    virtual int write_to_file(std::vector<std::pair<long, long>> data) = 0;

    /**
     * Retreive the value for a given key from the SST file
     * 
     * Note: Buffer Pool integration supported
     * 
     * @param key Search key
     * @param val_found Set to true if value found
     * 
     * @return Value found or -1 if value not found
     */
    virtual long search(long key, bool &val_found) = 0;

    /**
     * Scan a range of keys in from the SST file
     * 
     * Note: Buffer Pool integration not supported
     * 
     * @param result Stores results from the scan in this variable
     * @param key1 Search key, lower bound
     * @param key2 Search key, upper bound
     * 
     * @return Number of items found from scan
     */
    virtual int scan(std::vector<std::pair<long, long> > &result, long key1, long key2) = 0;

    /**
     * Delete the SST file associated with the SST object
     * 
     * @return 0 if SST file deleted successfully, -1 otherwise
     */
    virtual int delete_sst_file() = 0;

    /**
     * Read a single block from the SST file
     * 
     * @param buffer The data from the block/page will be placed in this buffer
     * @param block_num The index (in units of block/page) into the SST file to read 
     * 
     * @return The number of entries read from the block (the number of key-value pairs)
     */
    virtual int read_block(void * &buffer, long block_num) = 0;

    /**
     * Get the total number of blocks the SST file consists of
     * 
     * @return The total number of blocks the SST file consists of
     */
    virtual long get_num_blocks() = 0;

    /**
     * Get the total number of entries the SST file contains
     * 
     * @return The total number of entries the SST file contains
     */
    virtual long get_num_entries() = 0; 
};

/**
 * This class represents a SST file that uses the List SST structure
 */
class ListSST : public SST {
  public:
    ListSST(SSTOptions sst_options);
    ~ListSST() {}
    int write_to_file(std::vector<std::pair<long, long>> data);
    long search(long key, bool &val_found);
    int scan(std::vector<std::pair<long, long>> &result, long key1, long key2);
    int delete_sst_file();

    int read_block(void * &buffer, long block_num);
    long get_num_blocks();
    long get_num_entries();
  private:

    /**
     * Insert all keys contained in the SST file into the Bloom Filter
     * 
     * Note: Since Bloom Filter is not being persisted, we have to load in data manually into the Bloom Filter
     * 
     * @return 0 if successfully loaded, -1 otherwise
     */
    int load_bloom_filter();

    /**
     * SST configurations
     */
    SSTOptions options;

    /**
     * True if bloom filter is loaded, false otherwise
     */
    bool bloom_filter_loaded;
    
    /**
     * Bloom Filter
     */
    BloomFilter *bloom_filter;

};



#endif