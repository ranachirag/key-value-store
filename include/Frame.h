#ifndef FRAME_H
#define FRAME_H

#include <string>

/**
 * This class represents a frame storing the 1 page of data inside the Buffer Pool
 * It is also a node of a Linked list
 */
class Frame {
  public:
    /**
     * @brief Construct a new Frame object
     * 
     * @param hash_key - Hash key of the data
     * @param frame_data - Data to store
     * @param frame_data_size - Size of data (in bytes)
     */
    Frame(std::string hash_key, void *frame_data, int frame_data_size);

    /**
     * Hash key of the data
     */
    std::string hash_key;

    /**
     * Page data
     */
    void *data;
    
    /**
     * Size of data (in bytes)
     */
    int data_size;

    /**
     * Next Frame in the same bucket
     */
    Frame *next;

    /**
     * Metadata used for eviction policies
     */
    void *eviction_metadata;
};

#endif