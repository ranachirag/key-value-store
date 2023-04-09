#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <vector>
#include <cmath>

/**
 * Bloom Filter Configurations options
 */
struct BloomFilterOptions {
  double false_positive_rate;
  int bits_per_entry; // TODO: Implement
};

/**
 * This class represents a Bloom Filter
 */
class BloomFilter {
  public:
    /**
     * Construct a new Bloom Filter object
     * 
     * @param num_keys 
     * @param options 
     */
    BloomFilter(int num_keys, BloomFilterOptions options);

    /**
     * Given a vector of key-value pairs, insert all keys into the Bloom Filter
     * 
     * @param pairs Key-Value pairs
     * 
     * @return 0 if insert successful, -1 otherwose
     */
    int insert_keys(std::vector<std::pair<long, long>> pairs);

    /**
     * Check if key was inserted into the Bloom Filter
     * 
     * @param key The key to check
     * 
     * @return true - The key may or may not have been inserted into the Bloom Filter, subject to false positivity rate
     * @return false - The key was not inserted into the Bloom Filter
     */
    bool contains_key(long key);

    /**
     * Convert the bits in a vector of long type representation
     * 
     * Note: Unused function, used for Bloom Filter persistence which was not implemented
     * 
     * @param arr 
     * @return 0 if insert successful, -1 otherwose
     */
    int convert_bits_to_long_array(std::vector<unsigned long> &arr);

    /**
     * Read in the bits of the Bloom Filter from a vector of long types representation of the bits
     * 
     * Note: Unused function, used for Bloom Filter persistence which was not implemented
     * 
     * @param arr Vector of longs containing the bits in order
     * @param num_bits_to_add Number of bits to read
     * 
     * @return 0 if insert successful, -1 otherwose
     */
    int load_bits(std::vector<unsigned long> arr, int num_bits_to_add);

    /**
     * Read in the seed values of the Hash functions
     * 
     * @param arr Vector of seed values 
     * @param num_seed_values Number of seed values to read in
     * 
     * @return 0 if insert successful, -1 otherwose
     */
    int load_seed_values(std::vector<long> arr, int num_seed_values);

    /**
     * Number of bits the Bloom Filter is using
     */
    int num_bits;

    /**
     * Number of hash functions the Bloom filter is using
     */
    int num_hash_funcs;

    /**
     * Bits of the Bloom Filter
     */
    std::vector<bool> bits;

    /**
     * Seed values of the hash functions used by the Bloom Filter
     */
    std::vector<long> seed_values;

  private:
    /**
     * Bloom Filter configurations
     */
    BloomFilterOptions options;

    /**
     * Insert a single key into the Bloom Filter
     * 
     * @param key The key to insert
     * @return 0 if insert successful, -1 otherwose
     */
    int insert_key(long key);
};

#endif