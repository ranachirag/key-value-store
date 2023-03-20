#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <vector>

#define BLOCK_SIZE 4096
#define HASH_FUNC_SEED 443

class Frame {
  public:
    Frame(std::string hash_key, void *data);
    std::string hash_key;
    void *data;
    Frame *prev;
    Frame *next;
};

class Bucket {
  public:
    Bucket(Frame *frame, int frame_count, bool rehashed);

    Frame *frame;
  
    /**
     * Number of frames in the directory entry
     */
    int frame_count;

    /**
     * Determines whether the bucket has not been rehashed since the last time the directory was expanded
     */
    bool rehashed; 
};

class BufferPool {
  public:
    BufferPool(int initial_size, int max_size);
    int insert_data(std::string hash_key, void *data);
    int get_data(void * &data, std::string hash_key);
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
    
    bool expansion_required(float threshold);
    int get_offset(unsigned int hash_value);
    int update_dir_size(int new_size);
    int initialize_directory(int inital_size);
    int expand_directory();
    int rehash_bucket(Bucket *bucket);
    Bucket *get_bucket(std::string hash_key);
};

#endif