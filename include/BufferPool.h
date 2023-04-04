#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include <string>
#include <vector>
#include "Frame.h"
#include "Bucket.h"
#include "EvictionPolicy.h"

#include "DatabaseMacros.h"

struct BufferPoolOptions {
  /**
   * Intial size of the Buffer pool directory (in units of number of 4 KB pages it can store)
   * Note: The intial size must be a power of 2
   */
  int initial_size = 256; // 256 4 kB blocks, equivalent to 1 MB

  /**
   * Maximum size of the Buffer pool directory (in units of number of 4 KB pages it can store)
   * Note: The maximum size must be a power of 2
   */
  int max_size = 2048; // 256 4 kB blocks, equivalent to 8 MB

  /**
   * Eviction policy the Buffer pool uses.
   * Can only be "CLOCK" or "LRU"
   */
  std::string evict_policy = "CLOCK";
};

class BufferPool {
  public:
    /**
     * Construct a new Buffer Pool object
     * 
     * @param options Buffer Pool configurations
     */
    BufferPool(BufferPoolOptions options);

    /**
     * Insert a 4 KB page into the Buffer Pool
     * 
     * @param hash_key Hash key of the data 
     * @param data Pointer to the data 
     * @return 0 if successfully inserted, -1 otherwise
     */
    int insert_data(std::string hash_key, void *data);

    /**
     * @brief Get the data potentially stored in the Buffer Pool using hash key
     * 
     * @param hash_key Hash key of the data to retreive
     * @param data Pointer that is set to the data if found
     * @return 0 if data is found, -1 otherwise
     */
    int get_data(std::string hash_key, void * &data);

    /**
     * Update (grow/shrink) the Buffer Pool directory
     * 
     * Note: The new size of the Buffer Pool directory must be a power of 2
     * 
     * @param new_max_size New size maximum size of the Buffer Pool directory
     * @return 0 if successfully updated, -1 otherwise
     */
    int update_directory_size(int new_max_size);

    /**
     * Clear out all the pages in the Buffer Pool 
     */
    void reset_buffer_pool();

    /**
     * TODO: Remove This function, for Debugging purposes only
     * 
     */
    void print_bufpool();

  private:
    /**
     * Number of bits used to index into the directory at its current size
     */
    int prefix_length;

    /**
     * Current size of the directory (in units of # of entries) - equivalent to number of buckets
     */
    int num_buckets;

    /**
     * Maxium size the directory can expand to (in units of # of entries) - equivalent to maximum number of buckets
     */
    int max_num_buckets;

    /**
     * The amount of memory the directory takes up (in units of bytes) 
     * 
     * TODO: Remove this ?
     */
    int dir_size_bytes;

    /**
     * The number of frames in the directory
     */
    int num_frames;

    /**
     * The directory
     */
    std::vector<Bucket *> directory;

    /**
     * Eviction Policy, used to evict pages when the directory is full and cannot be expanded further
     */
    EvictionPolicy *eviction_policy;
    
    /**
     * Determines whether the Buffer Pool directory requires expansion bassed on the load factor threshold.
     * 
     * Note: Load factor = number of pages in the directory / number of buckets 
     *       For example, load factor threshold is set to 1 if ideally we want 1 frame per each bucket
     * 
     * @param threshold The th
     * @return true - If the directory requires expansion
     * @return false - If the directory does not require expansion
     */
    bool expansion_required(float threshold);

    /**
     * Initialize the buffer pool directory to hold initial size number of buckets
     * 
     * @param inital_size Initial size of the directory
     * @return 0 if initialized successfuly, -1 otherwise
     */
    int initialize_directory(int inital_size);

    /**
     * Expand the buffer pool directory to be double its current size
     * 
     * @return 0 if expanded successfully, -1 otherwise
     */
    int expand_directory();

    /**
     * Rehash a bucket if it has more than 1 frame and hasn't been rehashed since last time the buffer
     * pool directory was expanded.
     * 
     * @param bucket Bucket to rehash
     * @return 0 if rehashed successfully, -1 otherwise
     */
    int rehash_bucket(Bucket *bucket);

};

#endif