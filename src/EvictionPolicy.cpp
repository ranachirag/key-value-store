#include <iostream>
#include "EvictionPolicy.h"
#include "utils.h"
#include "DatabaseMacros.h"


EvictionNode::EvictionNode(Frame *frame) : frame(frame) {
  prev = nullptr;
  next = nullptr;
}

// ----------------- CLOCK -----------------

ClockEvictionPolicy::ClockEvictionPolicy() {
   zero = new int(0);
   one = new int(1);
}


int ClockEvictionPolicy::frame_created(Frame *frame) {
  frame->eviction_metadata = zero;
  if (clock_handle == nullptr) {
    clock_handle = frame;
  }
}

int ClockEvictionPolicy::frame_accessed(Frame *frame) {
  frame->eviction_metadata = one;
}

int ClockEvictionPolicy::evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length) {
  if(clock_handle == nullptr) {
    perror("No frame intialized");
    return -1;
  }
  
  unsigned int hash_value = hash_utils::get_hash_value(clock_handle->hash_key, HASH_FUNC_SEED);
  int handle_bucket_offset = buffer_pool_utils::get_offset(prefix_length, hash_value);

  int curr_bucket_num = handle_bucket_offset;
  int *clock_val = (int *) clock_handle->eviction_metadata;
  
  while(clock_val == one) {
    
    // Frame was accessed since last a frame was evicted, don't evict but set to 0 so it eligble for eviction 
    clock_handle->eviction_metadata = zero;
    
    // Moves clock_handle to point to next frame 
    // and curr_bucket_num to point to index into the directory of that next frame
    buffer_pool_utils::find_next_frame(directory, directory_size, clock_handle, curr_bucket_num);
    clock_val = (int *) clock_handle->eviction_metadata;
  
  }

  // Clock handle points to frame to be evicted
  Frame *frame_to_evict = clock_handle;
  int frame_to_evict_bucket_num = curr_bucket_num;
  Bucket *frame_to_evict_bucket = directory[frame_to_evict_bucket_num];

  // Move clock handle to next frame
  buffer_pool_utils::find_next_frame(directory, directory_size, clock_handle, curr_bucket_num);

  // Delete frame from bucket
  buffer_pool_utils::delete_frame(directory, directory_size, frame_to_evict_bucket, frame_to_evict);
  
  return 0;
}

// ------------------ LRU ------------------

LRUEvictionPolicy::LRUEvictionPolicy() {
  front = nullptr;
  back = nullptr;
}

int LRUEvictionPolicy::frame_created(Frame *frame) {
  EvictionNode *new_eviction_node = new EvictionNode(frame);
  frame->eviction_metadata = new_eviction_node;
  if(back == nullptr) {
    front = new_eviction_node;
    back = new_eviction_node;
  } else {
    back->next = new_eviction_node;
    new_eviction_node->prev = back;
    back = new_eviction_node;
  }
  EvictionNode *curr = front;
}

int LRUEvictionPolicy::frame_accessed(Frame *frame) {
  EvictionNode *eviction_node = (EvictionNode *) frame->eviction_metadata;
  if (eviction_node->next == nullptr) {
    return 0;
  }
  if(eviction_node->next && eviction_node->prev) {
    // In the middle of the Eviction Nodes list, update prev and next nodes
    eviction_node->prev->next = eviction_node->next;
    eviction_node->next->prev = eviction_node->prev;
  } else if (eviction_node->next) {
    // first node in the Eviction Nodes list, update front
    front = eviction_node->next;
    front->prev = nullptr;
  } 
  // Update the back
  back->next = eviction_node;
  eviction_node->prev = back;
  eviction_node->next = nullptr;
  back = eviction_node;
  return 0;
}

int LRUEvictionPolicy::evict_frame(std::vector<Bucket*> directory, int directory_size, int prefix_length) {
  if(front == nullptr) {
    perror("No frame to evict");
    return -1;
  }

  Frame *frame_to_evict = front->frame;

  // Remove EvictionNode from Queue
  EvictionNode *node_to_delete = front;
  front = node_to_delete->next;
  delete node_to_delete;

  unsigned int hash_value = hash_utils::get_hash_value(frame_to_evict->hash_key, HASH_FUNC_SEED);
  int bucket_offset = buffer_pool_utils::get_offset(prefix_length, hash_value);
  Bucket *frame_to_evict_bucket = directory[bucket_offset];

  // Delete frame from bucket
  buffer_pool_utils::delete_frame(directory, directory_size, frame_to_evict_bucket, frame_to_evict);

  return 0;
}