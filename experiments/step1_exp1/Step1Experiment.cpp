#include "Step1Experiment.h"
#include "BufferPool.h"
#include "Database.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>


int size_in_mb;
Database *db; 

Step1Experiment::Step1Experiment(int size, DatabaseOptions options) {
  size_in_mb = size;
  db = new Database(options);
  std::string db_name = "Step1Experiemnt_" + std::to_string(size) + "MB";
  db->open(db_name);
}

std::string Step1Experiment::putExperiment(){
    const int size = 65536 * size_in_mb;
    // int arr[size];
    int *arr = (int *) malloc(size * 8);

    // Initialize array values
    for (int i = 0; i < size; i++) {
        arr[i] = i + 1;
    }

    // Shuffle the array randomly
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr, arr + size, g);

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < size; i++){
      db->put(arr[i], i);
    }

    std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    std::cout << std::fixed << std::setprecision(9) << std::endl;
    double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << duration * 1000;
    free(arr);
    return std::to_string(duration * 1000);

};

std::string Step1Experiment::getExperiment(){
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  for (int i = 0; i < 1; ++i) {
    for(int j = 0; j < 10; ++j) {
      std::uniform_int_distribution<long> uni(0,65536 * size_in_mb);
      auto rand_int = uni(rng);
      db->get(rand_int);
    }
  }
std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
// std::cout << std::fixed << std::setprecision(9) << std::endl;
double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
return std::to_string(duration * 1000);

}

std::pair<std::string, std::string> Step1Experiment::scanExperiment(){
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  int total_entries = 0;
  int total_values = 65536 * size_in_mb;
  int start;
  int end;
  const int size = 5;
  std::pair<long, long> ranges[size];

  // Initialize array values
  // randomly generate value from 0% to 89% then add 10% and that is your range -- 10%
  std::uniform_int_distribution<int> dist(0, total_values - (total_values / 10));
  start = dist(rng);
  end = start + (total_values / 10);
  ranges[0] = std::make_pair(start, end);
  // randomly generate value from 0% to 74% then add 25% and that is your range -- 25%
  std::uniform_int_distribution<int> dist1(0, total_values - (total_values / 4));
  start = dist1(rng);
  end = start + (total_values / 4);
  ranges[1] = std::make_pair(start, end);
  // randomly generate value from 0% to 49% then add 50% and that is your range -- 50% 
  std::uniform_int_distribution<int> dist2(0, total_values - (total_values / 2));
  start = dist2(rng);
  end = start + (total_values / 2);
  ranges[2] = std::make_pair(start, end);
  // randomly generate value from 0% to 24% then add 75% and that is your range -- 75%
  // select 0 and 131072 * size_in_mb 
  std::uniform_int_distribution<int> dist3(0, (total_values / 4));
  start = dist3(rng);
  end = start + 3 * (total_values / 4);
  ranges[3] = std::make_pair(start, end);
  ranges[4] = std::make_pair(0, total_values);
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

  for (int i = 0; i < 5; i++) {
    std::vector<std::pair<long, long> > lst;
    int num_entries = db->scan(lst, ranges[i].first, ranges[i].second);
    total_entries += num_entries;
  }

  std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
  // std::cout << std::fixed << std::setprecision(9) << std::endl;
  double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
  // std::cout << duration * 1000 << ", " + std::to_string((duration * 1000) / total_entries) << std::endl;
  return std::make_pair (std::to_string(duration * 1000), std::to_string((duration * 1000) / total_entries)); 
}

void Step1Experiment::closedb() {
  db->close();
}


void Step1Experiment::run_experiments(int total_mb, int interval_mb) {
  int num_intervals = total_mb / interval_mb;
  std::random_device rd0;
  std::mt19937 g(rd0());
  int size = interval_mb * 65536;

  // add and measure time in intervals
  for (int k = 0; k < num_intervals; k++){

    int *arr = (int *) malloc(size * 8);

    for (int i = 0; i < size; i++) {
        arr[i] = i + 1;
    }

    std::shuffle(arr, arr + size, g);

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

    for (int i = 0; i < size; i++){
      db->put(arr[i], i);
    }

    std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << (duration * 1000) / size;
    free(arr);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<long> uni(0, (k + 1) * size);
    startTime = std::chrono::steady_clock::now();

    for(int j = 0; j < 10; ++j) {
      auto rand_int = uni(rng);
      // std::cout << rand_int << std::endl;
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

    for(int j = 0; j < 10; ++j) {
      auto rand_int1 = uni1(rdm);
      auto rand_int2 = uni2(rdm);
      // std::cout << rand_int << std::endl;
      std::vector<std::pair<long, long> > lst;
      total_entries += rand_int2 - rand_int1;
      db->scan(lst, rand_int1, rand_int2);
    }

    elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << "," << (duration * 1000) / total_entries;

    std::cout << std::endl;

  }
}