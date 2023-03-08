#ifndef STORAGE_H
#define STORAGE_H

#include <algorithm>
#include <vector>
#include <string>
#include "SST.h"

class Storage {
  private:
    std::string db_name;
    int num_SST();
    std::vector<SST*> SSTs;
    std::string get_SST_filename(); 
    std::string get_SST_filepath(std::string sst_filename);
    void create_SST(std::string filename); 
  public:
    Storage(std::string db_name);
    Storage(std::string db_name, std::vector<std::string> sst_files);
    int add_to_storage(std::vector<std::pair<long, long>> data);
    long get_value(long key, bool &val_found);
    std::vector<std::pair<long, long>> scan(long key1, long key2);
};

#endif