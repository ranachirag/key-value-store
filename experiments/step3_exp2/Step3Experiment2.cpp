#include "Step3Experiment2.h"
#include "BufferPool.h"
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <climits>
#include <chrono>


int size_in_mb;
Database *db; 

Step3Experiment2::Step3Experiment2(int size, DatabaseOptions options) {
  size_in_mb = size;
  db = new Database(options);
  std::string db_name = "Step3Experiment2_" + std::to_string(size) + "MB";
  db->open(db_name);
}

void Step3Experiment2::closedb() {
  db->close();
}

int bits_per_entry = 5;

void Step3Experiment2::run_experiments(int total_mb, int interval_mb) {
  int num_intervals = total_mb / interval_mb;
  std::random_device rd0;
  std::mt19937 g(rd0());
  int size = interval_mb * 65536;
  
  long global_min = LONG_MAX;
  long global_max = LONG_MIN;

  // add and measure time in intervals
  for (int k = 0; k < num_intervals; k++){

    std::cout << (k + 1) * interval_mb << ",";
    std::cout << bits_per_entry << ",";

    int *arr = (int *) malloc(size * 8);

    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }

    std::shuffle(arr, arr + size, g);


    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < size; i++){
      db->put(arr[i] + (k * size) + 1, i);
    }

    std::chrono::steady_clock::duration elapsedTime = std::chrono::steady_clock::now() - startTime;
    double duration = std::chrono::duration_cast< std::chrono::duration< double > >(elapsedTime).count();
    // std::cout << (duration * 1000) / size;
  
    free(arr);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<long> uni((k * size), (k + 1) * size * 2);
    startTime = std::chrono::steady_clock::now();

    for(int j = 0; j < 10; ++j) {
      auto rand_int = uni(rng);
      db->get(rand_int);
    }
    elapsedTime = std::chrono::steady_clock::now() - startTime;
    duration = std::chrono::duration_cast< std::chrono::duration< double > >(elapsedTime).count();
    std::cout << "," << (duration * 1000) / 10;

    bits_per_entry++;
    db->update_bloom_filter_param(bits_per_entry);

    std::cout << std::endl;
  }
}