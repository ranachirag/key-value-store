#ifndef STORAGE_H
#define STORAGE_H

#include <algorithm>
#include <vector>
#include <string>
#include "SST.h"

#define BLOCK_SIZE 4096

class Storage {
  private:
    std::string db_name;
    int num_SST;
    std::vector<SST> SSTs; 
  public:
    Storage(std::string db_name);
    Storage(std::string db_name, std::vector<std::string> SSTs);
    int create_SST(std::vector<std::pair<long, long>> data);
    SST read_SST();
};

#endif