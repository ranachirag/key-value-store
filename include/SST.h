#ifndef SST_H
#define SST_H

#include <string>

#define BLOCK_SIZE 4096

class SST {
  private:
    std::string filepath; // Path to the SST File
  public: 
    SST(std::string filepath) : filepath(filepath) {}
    long search(long key, bool &val_found);
    int scan(std::vector<std::pair<long, long>> &result, long key1, long key2);
};

// class SST_Array : public SST {
//   public:
//     SST_Array(std::string filepath) : SST(filepath) {}
//     long search(long key);
// };


// class SST_BTree : public SST {
//   public:
//     long search(long key);
// };


#endif