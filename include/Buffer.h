#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

class Frame {

};

struct dir_entry {
  Frame *frames;
  int frame_count;
};

class Buffer {
  public:
    Buffer(int intial_size, int max_size);
    int insert_data(std::string hash_key, void *data, int data_size);
    int get_data(void * &data, std::string hash_key);
  private:
    int prefix_length;
    int dir_size;
    int max_dir_size;
    dir_entry *directory;
    int get_offset(int hash_value);
    int update_dir_size(int new_size);
};

#endif