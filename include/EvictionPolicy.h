#ifndef EVICTION_POLICY_H
#define EVICTION_POLICY_H

#include <vector>
#include "Frame.h"
#include "Bucket.h"
#include "DatabaseMacros.h"

/**
 * This class represents an node in the Eviction queue (doubly Linked list) used in the LRU eviciton policy
 */
class EvictionNode {
  public:
    /**
     * Construct a new Eviction Node object
     * 
     * @param frame 
     */
    EvictionNode(Frame *frame);
    Frame *frame;
    EvictionNode *prev;
    EvictionNode *next;
};

/**
 * This abstract class represents an Eviction Polciy that is used to manage the eviction of frames from a Buffer Pool directory
 */
class EvictionPolicy  {
  public:
    /**
     * Select frame to evict and remove that frame from the buffer pool directory using eviction policy algorithm
     * 
     * @param directory Buffer Pool directory
     * @param directory_size Size of the Buffer Pool directory
     * @param prefix_length The number of prefix bits of the hash value the Buffer Pool is using
     * @return 0 if frame evicted successfully, -1 otherwise
     */
    virtual int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length) = 0;

    /**
     * This function is called when a frame is accessed, update metadata used by eviction policy algorithm
     * 
     * @param frame Frame that was accessed
     * @return 0 if metadata updated successfully, -1 otherwise
     */
    virtual int frame_accessed(Frame *frame) = 0;

    /**
     * This function is called when a frame is created, update metadata used by eviction policy algorithm
     * 
     * @param frame Frame that was created
     * @return 0 if metadata updated successfully, -1 otherwise 
     */
    virtual int frame_created(Frame *frame) = 0;
};

/**
 * This class represents the CLOCK Eviction Policy
 */
class ClockEvictionPolicy : public EvictionPolicy {
  public:
    ClockEvictionPolicy();
    int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length);
    int frame_accessed(Frame *frame);
    int frame_created(Frame *frame);
  private:
    Frame *clock_handle;
    int *zero;
    int *one;
};

/**
 * This class represents the LRU Eviction Policy
 */
class LRUEvictionPolicy : public EvictionPolicy {
  public:
    LRUEvictionPolicy();
    int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length);
    int frame_accessed(Frame *frame);
    int frame_created(Frame *frame);
  private:
    EvictionNode *front;
    EvictionNode *back;
};


#endif
