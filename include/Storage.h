#ifndef STORAGE_H
#define STORAGE_H

#include <algorithm>
#include <list>
#include "SST.h"

#define BLOCK_SIZE 4096

class Storage {
  private:
    SST sst[]; 
  public:
    int create_SST(std::list<std::pair<long int, long int>> data);
    SST open_SST();
};

#endif