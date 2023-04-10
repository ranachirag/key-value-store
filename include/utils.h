#include <string>
#include <vector>

#include "Frame.h"
#include "Bucket.h"
#include "BufferPool.h"
#include "Storage.h"
#include "SST.h"
#include "DatabaseMacros.h"

/**
 * This namespace contains utility functions dealing with Files and Buffers
 */
namespace file_utils {

  /**
   * Compare age of SST files by their filename
   * 
   * @param file_a File A
   * @param file_b File B
   * 
   * @return true - If File A is older
   * @return false - If File A is younger
   */
  bool sst_filename_compare(std::string file_a, std::string file_b);

  /**
   * Get the file size of a given file
   * 
   * @param filepath Filepath of the file
   * 
   * @return The size of the file in bytes
  */
  long get_filesize(std::string filepath);

  /**
   * Reinterpret buffer as Key-Value pairs
   * 
   * @param buffer Buffer to reinterpret
   * @param bytes_read Number of bytes to reintrepret
   * 
   * @return Key-Value representation of the buffer
   */
  std::vector<std::pair<long, long>> reintrepret_buffer(void *buffer, int bytes_read);

  /**
   * Read a single block from a binary file.
   * 
   * @param buffer Buffer to place the block read from binary file in
   * @param filepath Path of file to read block from
   * @param block_num The index of block to read from (zero-indexed)
   * 
   * @return Number of bytes read from the block
   */
  int read_block(void * &buffer, std::string filepath, long block_num);

  /**
   * Read a single block from a binary file with the support of a buffer pool.
   * 
   * @param buffer_pool Buffer pool for quicker access of blocks
   * @param buffer Buffer to place the block read from binary file in
   * @param filepath Path of file to read block from
   * @param block_num The index of block to read from (zero-indexed)
   * 
   * @return Number of bytes read from the block
   */
  int read_block_buffer_pool(BufferPool *buffer_pool, void * &buffer, std::string filepath, long block_num);

  /**
   * Write data to a binary file (in units of blocks).
   * 
   * @param filepath Path of file to read block from
   * @param data The index of block to read from (zero-indexed)
   * 
   * @return 0 if successful write, -1 if unsuccessful write
   */
  int write_data(std::string filepath, long *data, long size);
}

/**
 * This namespace contains utility functions dealing with Search algorithms
 */
namespace search_utils {

  /**
   * Binary search for a key on a sorted file (in units of block)
   * 
   * @param filepath Path of file to 
   * @param num_blocks Number of blocks in the file
   * @param key Key to find
   * 
   * @return Index of the block that contains key, -1 if not found 
   */
  long binary_search_blocks(std::string filepath, long num_blocks, long key);
  
  /**
   * Binary search for a key range on a sorted file (in units of block)
   * 
   * @param filepath Path of file to 
   * @param num_blocks Number of blocks in the file
   * @param key1 Start key of range
   * @param key2 End key of range
   * 
   * @return Index of the first block that has keys in the key range, -1 if not found 
   */
  long binary_search_range_blocks(std::string filepath, long num_blocks, long key1, long key2);

  /**
   * Binary search for a key on a sorted vector (in units of key value pairs)
   * 
   * @param key_values Vector to perform binary serach on
   * @param key Key to find
   * 
   * @return Index of the key-value pair that matches search key, -1 if not found
   */
  long binary_search_kv(std::vector<std::pair<long, long> > key_values, long key);
}

/**
 * This namespace represents functions dealing with Hashing
 */
namespace hash_utils {
  /**
   * Get a 32 bit hash value from a provided string (hash key)
   * 
   * @param hash_key Key to hash
   * @param seed Seed used for xxHash function
   * 
   * @return Hash Value
   */
  int get_hash_value(std::string hash_key, int seed);
}

/**
 * This namespace represents functions dealing with Numbers/Integers
 */
namespace math_utils {
  /**
   * Get the number of bits it takes to represent a given 32-bit integer 
   * Note: Assumes value is 0-indexed, calculates number of bits takes to represent value-1
   * 
   * @param value 32-bit integer 
   * 
   * @return Number of bits it takes to represent a given 32-bit integer 
  */
  int get_num_bits(unsigned int value);

  /**
   * Get the first n bits from a given 32-bit integer
   * 
   * @param value 32-bit integer
   * @param num_bits Number for prefix bits to retreive
   * 
   * @return First n bits from a given 32-bit integer
   */
  int get_prefix_bits(unsigned int value, unsigned int num_bits);
}

/**
 * This namespace represents functions dealing with the Buffer Pool object
 */
namespace buffer_pool_utils {
  
  /**
   * Get the offset into the Directory from prefix of the hash value
   * 
   * @param prefix_length Length of the prefix of the hash value
   * @param hash_value Hash value
   * 
   * @return Offset into the Directory
   */
  int get_offset(int prefix_length, unsigned int hash_value);

  /**
   * Get the hash bucket with the given hash key
   * 
   * @param directory Directory
   * @param prefix_length Length of the hash value to use to index into the directory
   * @param hash_key Hash key to use to find hash bucket
   * 
   * @return Hash bucket of the given hash key
   */
  Bucket *get_bucket(const std::vector<Bucket *> directory, int prefix_length, std::string hash_key);

  /**
   * Rehash a hash bucket  
   * 
   * @param directory Directory
   * @param directory_size Size of the directory 
   * @param prefix_length Length of the hash value to use to index into the directory
   * @param bucket Hash bucket to rehash
   * 
   * @return 0 if successfuly, -1 otherwise 
   */
  int rehash_bucket(std::vector<Bucket *> directory, int directory_size, int prefix_length, Bucket *bucket);

  /**
   * Insert a frame into a hash bucket
   * 
   * @param bucket Hash bucket
   * @param frame_to_insert Frame to insert
   * 
   * @return 0 if successfuly, -1 otherwise
   */
  int insert_frame(Bucket *bucket, Frame *frame_to_insert);
  
  /**
   * Delete a frame from the directory
   * 
   * @param directory Directory
   * @param directory_size Size of the directory (in number of buckets)
   * @param dir_size_bytes Size of the directory (in bytes)
   * @param bucket Hash bucket to delete frame from
   * @param frame Frame to delete
   * 
   * @return 0 if successfuly, -1 otherwise
   */
  int delete_frame(std::vector<Bucket *> directory, int directory_size, int &dir_size_bytes, Bucket *bucket, Frame *frame);

  /**
   * Algorithm that iterates through the directory (Clock algorithm)
   * Starting from a given frame and ending when it reaches the next fram
   * 
   * @param directory Directory
   * @param directory_size Size of the directory
   * @param frame Frame to start search with, will be set to next frame after search completes
   * @param bucket_num Bucket index into the directory that contains the starting frame, will be set to bucket index of next frame after search completes
   */
  void find_next_frame(const std::vector<Bucket *> directory, int directory_size, Frame * &frame, int &bucket_num);
}


/**
 * This namespace represents functions dealing with Option structs
 */
namespace options_utils {
  /**
   * Generates configuration options for LSM Level object, given Storage configuration
   * 
   * @param options Storage configuration options
   * @param level_num The number of the level in the LSM tree
   * 
   * @return LSM Tree level configuration options
   */
  LevelLSMOptions storage_to_level(StorageOptions options, int level_num);

  /**
   * Generates configuration options for the next LSM Level in the LSM Tree, given current LSM Tree level configuration
   * 
   * @param options LSM Tree level configuration options for current level
   * @param size_ratio The size ratio of SST file sizes between levels
   * 
   * @return LSM Tree level configuration options for next level
   */
  LevelLSMOptions level_to_next_level(LevelLSMOptions options, int size_ratio);

  /**
   * Generates configuration options for a SST object, given a LSM Tree level configuration 
   * 
   * @param options LSM Tree level configuration options 
   * @param filepath File path of the SST file
   * @param sst_capacity Capacity of the SST file (maximum number of entries it can hold)
   * @param file_exists True if the file for the SST object exists, false otherwise
   * 
   * @return SST configuration options
   */
  SSTOptions level_to_sst(LevelLSMOptions options, std::string filepath, int sst_capacity, bool file_exists);
}

/**
 * This namespace represents functions dealing with Option structs
 */
namespace sort_utils {
  /**
   * Compare Key Value pair (for sorting)
   * 
   * @param a First Key value pair
   * @param b Second Key value pair
   * 
   * @return true If a's key is less than b's key
   * @return false If a's key is greater than b's key
   */
  bool compare_kv_pair(const std::pair<long, long>& a, const std::pair<long, long>& b);
}

/**
 * This namespace represents functions dealing with Bloom Filter
 */
namespace bloom_filter_utils {

  /**
   * Set the Bloom Filter paramters to give the provided False Positive rate
   * 
   * @param num_keys Number of keys to be inserted into the Bloom Filter
   * @param false_positive_rate False positive rate
   * @param num_bits Number of bits the Bloom Filter uses, parameter to be set
   * @param num_hash_funcs Number of hash function the Bloom Filter uses, parameter to be set
   * 
   * @return 0 if successfull, -1 otherwise
   */
  int set_bloom_filter_params(int num_keys, double false_positive_rate, int &num_bits, int &num_hash_funcs);

  /**
   * Set the Bloom Filter paramters given the number of bits per entry it should use
   * 
   * @param num_keys Number of keys to be inserted into the Bloom Filter
   * @param bits_per_entry Bits per entry
   * @param num_bits Number of bits the Bloom Filter uses, parameter to be set
   * @param num_hash_funcs Number of hash function the Bloom Filter uses, parameter to be set
   * 
   * @return 0 if successfull, -1 otherwise
   */
  int set_bloom_filter_params(int num_keys, int bits_per_entry, int &num_bits, int &num_hash_funcs);
}