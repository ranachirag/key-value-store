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
     * @param frame The frame (equivalently a page)
     */
    EvictionNode(Frame *frame);

    /**
     * The frame (equivalently a page) 
     */
    Frame *frame;

    /**
     * Previous EvictionNode in the Linked list
     */
    EvictionNode *prev;

    /**
     * Next EvictionNode in the Linked list
     */
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
     * @param directory_size Size of the Buffer Pool directory (in number of buckets)
     * @param prefix_length Number of prefix bits of the hash value used to index into the Directory
     * @param dir_size_bytes Size of the Buffer Pool directory (in bytes)
     * 
     * @return 0 if frame evicted successfully, -1 otherwise
     */
    virtual int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length, int &dir_size_bytes) = 0;

    /**
     * This function is called when a frame is accessed, update metadata used by eviction policy algorithm
     * 
     * @param frame Frame that was accessed
     * 
     * @return 0 if metadata updated successfully, -1 otherwise
     */
    virtual int frame_accessed(Frame *frame) = 0;

    /**
     * This function is called when a frame is created, initialize metadata used by eviction policy algorithm
     * 
     * @param frame Frame that was created
     * 
     * @return 0 if metadata initialized successfully, -1 otherwise 
     */
    virtual int frame_created(Frame *frame) = 0;
};

/**
 * This class represents the CLOCK Eviction Policy
 */
class ClockEvictionPolicy : public EvictionPolicy {
  public:
    /**
     * Construct a new Clock Eviction Policy object
     */
    ClockEvictionPolicy();

    /**
     * Evict a frame using the Clock Eviction algorithm
     * 
     * @param directory Buffer Pool directory
     * @param directory_size Size of the Buffer Pool directory (in number of buckets)
     * @param prefix_length Number of prefix bits of the hash value used to index into the Buffer Pool directory
     * @param dir_size_bytes Size of the Buffer Pool directory (in bytes)
     * 
     * @return 0 if successful, -1 otherwise
     */
    int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length, int &dir_size_bytes);

    /**
     * This function is called when a frame is accessed, update frame metadata
     * Note: In the case of the Clock Eviction Policy, the metadata is simply a bit value that is set to 1
     *       The bit value is represented by an integer pointer
     * 
     * @param frame Frame that was accessed
     * 
     * @return 0 if metadata updated successfully, -1 otherwise
     */
    int frame_accessed(Frame *frame);

    /**
     * This function is called when a frame is created, initilize frame metadata
     * Note: In the case of the Clock Eviction Policy, the metadata is simply a bit value that is initialized to 0
     *       The bit value is represented by an integer pointer
     * 
     * @param frame Frame that was created
     * 
     * @return 0 if metadata initialized successfully, -1 otherwise 
     */
    int frame_created(Frame *frame);

  private:
    /**
     * Points to the frame in the directory that is the current Clock handle
     */
    Frame *clock_handle;

    /**
     * Represents the bit value 0
     */
    int *zero;

    /**
     * Represents the bit value 1
     */
    int *one;
};

/**
 * This class represents the LRU Eviction Policy
 */
class LRUEvictionPolicy : public EvictionPolicy {
  public:
    /**
     * Construct a new LRUEvictionPolicy object
     */
    LRUEvictionPolicy();

    /**
     * Evict a frame using the LRU Eviction algorithm
     * 
     * @param directory Buffer Pool directory
     * @param directory_size Size of the Buffer Pool directory (in number of buckets)
     * @param prefix_length Number of prefix bits of the hash value used to index into the Buffer Pool directory
     * @param dir_size_bytes Size of the Buffer Pool directory (in bytes)
     * 
     * @return 0 if successful, -1 otherwise
     */
    int evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length, int &dir_size_bytes);

    /**
     * This function is called when a frame is accessed, update frame metadata
     * Note: In the case of the LRU Eviction Policy, the metadata is a pointer to the corresponding 
     *       EvictionNode object for the frame. The EvictionNode object is moved to the back of the queue
     *       when the corresponding Frame is accessed.
     * 
     * 
     * @param frame Frame that was accessed
     * 
     * @return 0 if metadata updated successfully, -1 otherwise
     */
    int frame_accessed(Frame *frame);

    /**
     * This function is called when a frame is created, update frame metadata
     * Note: In the case of the LRU Eviction Policy, the metadata is a pointer to the corresponding 
     *       EvictionNode object for the frame. The corresponding EvictionNode object is created for 
     *       the frame and the metadata is updated to be set to the pointer to the newly created EvictionNode
     * 
     * 
     * @param frame Frame that was accessed
     * 
     * @return 0 if metadata updated successfully, -1 otherwise
     */
    int frame_created(Frame *frame);

  private:
    /**
     * Pointer to the EvictionNode at the front of the queue
     */
    EvictionNode *front;

    /**
     * Pointer to the EvictionNode at the back of the queue
     */
    EvictionNode *back;
};


#endif
