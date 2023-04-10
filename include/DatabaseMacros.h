#ifndef DATABASE_MACROS_H
#define DATABASE_MACROS_H

/**
 * Size of a Key (in bytes)
 */
#define KEY_SIZE 8

/**
 * Size of a Value (in bytes)
 */
#define VALUE_SIZE 8

/**
 * Size of 1 block/page
 */
#define BLOCK_SIZE 4096

/**
 * Seed used in the xxHash functions
 */
#define HASH_FUNC_SEED 443

/**
 * Threshold for expanding Buffer Pool directory
 * 
 * For example, a value of 1 means the directory is expanded whenever the average number of 
 * frames for each bucket exceeds 1.
 */
#define LOAD_FACTOR_THRESHOLD 1

/**
 * CLOCK eviction policy option
 */
#define CLOCK_EVICT "CLOCK"

/**
 * LRU eviction policy option
 */
#define LRU_EVICT "LRU"

/**
 * List SST structure
 */
#define LIST_SST "LIST"

/**
 * B-Tree SST structure
 */
#define BTREE_SST "BTREE"

/**
 * Append Only Storage option
 */
#define APPEND_ONLY_STORAGE "APPEND_ONLY_STORAGE"

/**
 * LSM Tree Storage option
 */
#define LSM_TREE_STORAGE "LSM_TREE_STORAGE"

/**
 * Threshold to compact a level
 * 
 * Equivalent to the size ration between 2 levels
 */
#define SIZE_RATIO 2

/**
 * Use Bits per entry to set Bloom Filter paramters
 */
#define BITS_PER_ENTRY "BITS_PER_ENTRY"

/**
 * Use False positive rate to set Bloom Filter paramters
 */
#define FALSE_POSITIVE_RATE "FALSE_POSITIVE_RATE"

#define NUM_BYTES_MB 1048576

#endif