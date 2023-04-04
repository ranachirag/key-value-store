#ifndef DATABASE_MACROS_H
#define DATABASE_MACROS_H

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

#endif