#include "Step1Experiment.h"
#include "BufferPool.h"
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <random>


int size_in_mb;
Database *db; 

Step1Experiment::Step1Experiment(int size, DatabaseOptions options) {
  size_in_mb = size;
  db = new Database(options);
  std::string db_name = "Step1Experiemnt_" + std::to_string(size) + "MB";
  db->open(db_name);
}

void Step1Experiment::putExperiment(){
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  for (int i = 0; i < 1; ++i) {
    for(int j = 0; j < 131072 * size_in_mb; ++j) {
      int key_val = i*131072 + j;
      db->put(key_val, key_val);
    }
  }
std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
std::cout << std::fixed << std::setprecision(9) << std::endl;
double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
std::cout << std::to_string(size_in_mb) + "MB Put Milliseconds: " << duration * 1000 << std::endl;

}

void Step1Experiment::getExperiment(){
  int dnevalues = 0;
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  for (int i = 0; i < 1; ++i) {
    for(int j = 0; j < 10; ++j) {
      std::uniform_int_distribution<int> uni(0,131072 * size_in_mb);
      auto rand_int = uni(rng);
      if (db->get(rand_int) == -1) {
        dnevalues += 1;
      }
      // std::cout << rand_int << std::endl;
    }
  }
std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
std::cout << std::fixed << std::setprecision(9) << std::endl;
double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
std::cout << std::to_string(size_in_mb) + "MB Get Milliseconds: " << duration * 1000 << std::endl;
std::cout << std::to_string(size_in_mb) + "MB Get DNE values: " << std::to_string(dnevalues) << std::endl;

}

void Step1Experiment::scanExperiment(){
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  for (int i = 0; i < 1; ++i) {
    for(int j = 0; j < 10; ++j) {
      std::uniform_int_distribution<int> uni(0,131072 * size_in_mb);
      auto rand_int1 = uni(rng);
      auto rand_int2 = uni(rng);
      std::vector<std::pair<long, long> > lst;
      db->scan(lst, rand_int1, rand_int2);
      }
      // std::cout << rand_int << std::endl;
  }
std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
std::cout << std::fixed << std::setprecision(9) << std::endl;
double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
std::cout << std::to_string(size_in_mb) + "MB Scan Milliseconds: " << duration * 1000 << std::endl;

}

void Step1Experiment::closedb() {
  db->close();
}
