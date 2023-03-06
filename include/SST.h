#ifndef SST_H
#define SST_H

#include <string>

class SST {
  public: 
    std::string filepath; // Path to the SST File
    SST(std::string filepath);
    virtual long search(long key);
};

class SST_Array : public SST {
  public:
    SST_Array(std::string filepath);
    std::string filepath; // How to Inherit from parent class?
    long search(long key);
};


// class SST_BTree : public SST {
//   public:
//     long search(long key);
// };


#endif