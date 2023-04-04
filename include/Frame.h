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
     */
    Frame(std::string hash_key, void *frame_data);

    /**
     * Hash key of the data
     */
    std::string hash_key;

    /**
     * Page data
     */
    void *data;

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