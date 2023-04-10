#include <iostream>
#include "Database.h"
#include "AVL.h"
#include <algorithm>
#include <vector>
#include <string>
#include "utils.h"
#include "BufferPool.h"
#include <chrono>
#include <iomanip>
#include "Step3Experiment1.h"
#include "DatabaseMacros.h"
#include <fstream>

int main() {

  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB Buffer Pool size limit
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5; // 5 bits per entry for the Bloom Filter

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1; // 1 MB 
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;

  int arr_size = 8;

  // int sizes[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
  int sizes[8] = {2, 4, 8, 16, 32, 64, 128, 256};
  std::string* putValues = new std::string[8];
  std::string* getValues = new std::string[8];
  std::pair<std::string, std::string> scanValues[8];
  for (int i = 0; i < arr_size; i++){
    Step3Experiment1 *exp = new Step3Experiment1(sizes[i], db_options);
    putValues[i] = exp->putExperiment();
    getValues[i] = exp->getExperiment();
    scanValues[i] = exp->scanExperiment();
  }
  std::ofstream putFile("put.txt");
  if (!putFile.is_open()) {
    std::cout << "Error: Could not open file for writing" << std::endl;
    return 1;
  }
  for (int i = 0; i < arr_size; i++) {
    putFile << std::to_string(sizes[i]) << ", " << putValues[i] << std::endl;
  }
  putFile.close();

  std::ofstream getFile("get.txt");
  if (!getFile.is_open()) {
    std::cout << "Error: Could not open file for writing" << std::endl;
    return 1;
  }
  for (int i = 0; i < arr_size; i++) {
    getFile << std::to_string(sizes[i]) << ", " << getValues[i] << std::endl;
  }
  getFile.close();

  std::ofstream scanFile("scan.txt");
  if (!scanFile.is_open()) {
    std::cout << "Error: Could not open file for writing" << std::endl;
    return 1;
  }
  for (int i = 0; i < arr_size; i++) {
    scanFile << std::to_string(sizes[i]) << ", " << scanValues[i].first + ", " + scanValues[i].second << std::endl;
  }
  scanFile.close();

}