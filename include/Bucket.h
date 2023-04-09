#ifndef BUCKET_H
#define BUCKET_H

#include "Frame.h"

/**
 * This class represents a hash bucket in the Buffer Pool directory
 */
class Bucket {
  public:
    /**
     * Construct a new Bucket object
     * 
     * @param frame Linked list of the frames the bucket holds
     * @param frame_count The number of frames the bucket holds
     * @param rehashed True if bucket has been rehashed, false otherwise
     */
    Bucket(Frame *frame, int frame_count, bool rehashed);

    /**
     * Linked list of frames the bucket holds
     */
    Frame *frame;
  
    /**
     * Number of frames in the bucket
     */
    int frame_count;

    /**
     * Determines whether the bucket has not been rehashed since the last time the directory was expanded
     */
    bool rehashed; 

};

#endif