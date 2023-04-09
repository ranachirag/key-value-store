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

void Step1Experiment::putExperiment(){
    const int size = 131072;
    int arr[size];

    // Initialize array values
    for (int i = 0; i < size; i++) {
        arr[i] = i + 1;
    }

    // Shuffle the array randomly
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(arr, arr + size, g);

    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    for (int j = 0; j < size_in_mb; j++){
      for (int i: arr) {
        db->put(i + (j * 131072), i);
      }
    }

    std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
    std::cout << std::fixed << std::setprecision(9) << std::endl;
    double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
    std::cout << duration * 1000;

};

void Step1Experiment::getExperiment(){
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
  for (int i = 0; i < 1; ++i) {
    for(int j = 0; j < 10; ++j) {
      std::uniform_int_distribution<int> uni(0,131072 * size_in_mb);
      auto rand_int = uni(rng);
      db->get(rand_int);
    }
  }
std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
std::cout << std::fixed << std::setprecision(9) << std::endl;
double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
std::cout << duration * 1000;

}

void Step1Experiment::scanExperiment(){
  std::random_device rd;     // Only used once to initialise (seed) engine
  std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
  int total_entries = 0;
  int total_values = 131072 * size_in_mb;
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
    db->scan(lst, ranges[i].first, ranges[i].second);
    total_entries += ranges[i].second - ranges[i].first;
  }

  std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
  std::cout << std::fixed << std::setprecision(9) << std::endl;
  double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
  std::cout << duration * 1000 << ", " + std::to_string((duration * 1000) / total_entries) << std::endl;
}

void Step1Experiment::closedb() {
  db->close();
}
