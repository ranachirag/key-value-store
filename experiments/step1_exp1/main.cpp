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
#include "Step1Experiment.h"
#include "DatabaseMacros.h"
#include <fstream>

int main() {

  
  DatabaseOptions db_options;
  db_options.use_buffer_pool = false;
  db_options.use_bloom_filters = false;
  
  db_options.memtable_size = NUM_BYTES_MB * 1;
  
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = APPEND_ONLY_STORAGE;

  int arr_size = 8;

  // int sizes[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
  int sizes[8] = {2, 4, 8, 16, 32, 64, 128, 256};
  std::string* putValues = new std::string[8];
  std::string* getValues = new std::string[8];
  std::pair<std::string, std::string> scanValues[8];
  for (int i = 0; i < arr_size; i++){
    Step1Experiment *exp = new Step1Experiment(sizes[i], db_options);
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