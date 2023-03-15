#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>

class FrameNode {

};

struct dir_entry {
  FrameNode *frames;
  int frame_count;
};

class Directory {
  public:
    Directory(int intial_size, int max_size);
    int insert_data(std::string hash_key, void *data, int data_size);
    int get_data(void * &data, std::string hash_key);
  private:
    int prefix_length;
    int dir_size;
    int max_dir_size;
    dir_entry *directory;
    int get_hash_value(std::string hash_key);
    int get_offset(int hash_value);
};

#endif