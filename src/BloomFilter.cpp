#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include <bitset>

#include "xxhash.h"
#include "BloomFilter.h"


int caclulate_bloom_filter_params(int num_keys, double false_positive_rate, int &num_bits, int &num_hash_funcs) {
  int n = num_keys;

  num_bits = (-1 * n * log(false_positive_rate)) / (log(2) / 2);

  int m = num_bits;
  int k = (m / n) * log(2);

  num_hash_funcs = k;

  return 0;
}

BloomFilter::BloomFilter(int num_keys, BloomFilterOptions options) : options(options) {
  // Using https://en.wikipedia.org/wiki/Bloom_filter#Optimal_number_of_hash_functions to determine the optimal number of bits and hash functions to use
  
  caclulate_bloom_filter_params(num_keys, options.false_positive_rate, num_bits, num_hash_funcs);

  bits = std::vector<bool>(num_bits, false);

  // Set seed of RNG with the current time
  std::srand(std::time(nullptr)); 

  for(int i = 0; i < num_hash_funcs; ++i) {
    int random_num = std::rand();
    seed_values.push_back((long) random_num);
  }

}


int BloomFilter::insert_key(long key) {
  std::string key_as_string = std::to_string(key);
  for(int i = 0; i < num_hash_funcs; ++i) {
    long hash_value = XXH64(key_as_string.c_str(), key_as_string.length(), seed_values[i]) % num_bits;
    bits[hash_value] = true;
  }
}

int BloomFilter::insert_keys(std::vector<std::pair<long, long>> pairs) {
  for(std::pair<long, long> pair: pairs) {
    long key = pair.first;
    insert_key(key);
  }
}

bool BloomFilter::contains_key(long key) {
  std::string key_as_string = std::to_string(key);
  int key_length = key_as_string.length();
  for(int i = 0; i < num_hash_funcs; ++i) {
    long hash_value = XXH64(key_as_string.c_str(), key_length, seed_values[i]) % num_bits;
    bool bit_val = bits[hash_value];

    // Bit wasn't set by the hash function, therefore key must not be contained
    if(!bit_val) {
      return false;
    }
  }
  // All bits were set by the hash functions, key may or may not be inserted into Bloom filter
  return true;
}


int BloomFilter::convert_bits_to_long_array(std::vector<unsigned long> &arr) {
  int num_bits_in_long = sizeof(long) * 8;
  int num_longs = (num_bits + num_bits_in_long - 1) / num_bits_in_long;
  
  int bits_to_be_inserted = num_bits;
  for(int i = 0; i < num_longs; ++i) {
    long long_to_insert = 0;
    int num_bits_to_insert = std::min(bits_to_be_inserted, num_bits_in_long);
    for(int j = 0; j < num_bits_to_insert; ++j) {
      long_to_insert |= static_cast<long>(bits[(i * num_bits_in_long) + j]) << j;
    }
    arr.push_back(long_to_insert);
    bits_to_be_inserted -= num_bits_to_insert;
  }
}


int BloomFilter::load_bits(std::vector<unsigned long> arr, int num_bits_to_add) {
  int count = 0;
  for(long val: arr) {
    for (int i = 0; i < sizeof(long) * 8; ++i) {
        if(count != num_bits_to_add) {
          bits.push_back(((val >> i) & 1));
          count++;
        }
    }
  }
  num_bits = num_bits_to_add;
}

int BloomFilter::load_seed_values(std::vector<long> arr, int num_seed_values) {
  int count = 0;
  for(int val: arr) {
    seed_values.push_back(val);
  }
  num_hash_funcs = num_seed_values;
}