#include "Step1Experiment.h"
#include "BufferPool.h"
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>


int size_in_mb;
Database *db; 

Step3Experiment2::Step3Experiment2(int size, DatabaseOptions options) {
  size_in_mb = size;
  db = new Database(options);
  std::string db_name = "Step3Experiment2_" + std::to_string(size) + "MB";
  db->open(db_name);
}

void Step1Experiment::closedb() {
  db->close();
}


void Step1Experiment::run_experiments(int total_mb, int interval_mb) {
  int num_intervals = total_mb / interval_mb;
  std::random_device rd0;
  std::mt19937 g(rd0());
  int size = interval_mb * 65536;
  // 4194304
  long global_min = LONG_MAX;
  long global_max = LONG_MIN;

  // add and measure time in intervals
  for (int k = 0; k < num_intervals; k++){

    std::cout << (k + 1) * interval_mb << ",";

    int *arr = (int *) malloc(size * 8);
    // 3344554432

    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }

    std::shuffle(arr, arr + size, g);


    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < size; i++){
      db->put(arr[i] + (k * size) + 1, i);
    }

    std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << (duration * 1000) / size;
  
    long min_val = k * size + 1;
    long max_val = (k + 1) * size;

    if (min_val < global_min){
      global_min = min_val;
    }
    if (max_val > global_max){
      global_max = max_val;
    }
    free(arr);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<long> uni(0, (k + 1) * size);
    startTime = std::chrono::steady_clock::now();

    for(int j = 0; j < 10; ++j) {
      auto rand_int = uni(rng);
      db->get(rand_int);
    }
    elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << "," << (duration * 1000) / 10;

    // scan 

    std::random_device rd2;
    std::mt19937 rdm(rd2());
    std::uniform_int_distribution<long> uni1(0, ((k + 1) * size) / 2);
    std::uniform_int_distribution<long> uni2(((k + 1) * size) / 2, (k + 1) * size);

    startTime = std::chrono::steady_clock::now();
    int total_entries = 0;

    std::vector<std::pair<long, long>> lst(0);
    total_entries = db->scan(lst, global_min, global_max);


    elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    duration = ::std::chrono::duration_cast< ::std::chrono::duration< double >>(elapsedTime).count();
    std::cout << "," << (duration * 1000) / total_entries;

    std::cout << std::endl;
    std::cout << "total entries " + std::to_string(lst.size()) + "," << std::to_string(global_min) + "," + std::to_string(global_max) << std::endl;
  }
}