#include <string>
#include <vector>

namespace file_utils {
  /**
   * Get the file size of a given file
   * 
   * @param filepath Filepath of the file
   * 
   * @return The size of the file in bytes
  */
  long get_filesize(std::string filepath);

  /**
   * Read a single block from a binary file.
   * 
   * @param filepath Path of file to read block from
   * @param block_num The index of block to read from (zero-indexed)
   * 
   * @return All key value pairs in the specified block
   */
  std::vector<std::pair<long, long> > read_block(std::string filepath, long block_num);

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

namespace math_utils {
  /**
   * Get the number of bits it takes to represent a given 32-bit integer 
   * 
   * @param value 32-bit integer 
   * 
   * @return Number of bits it takes to represent a given 32-bit integer 
  */
  int get_num_bits(int value);
}