#include "Buffer.h"
#include "utils.h"

Buffer::Buffer(int intial_size, int max_size) {
  dir_size = 0;
  max_dir_size = max_size;

  // Number of bits, each 
  prefix_length = math_utils::get_num_bits(dir_size);

  // TODO: Create a func to initialize Directory

}
