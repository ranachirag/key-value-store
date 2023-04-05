#include "Step1Experiment.h"
#include "Bufferpool.h"
#include "Database.h"
#include <iomanip>
#include <iostream>

class Step1Experiment {
  int size_in_mb;
  Database *db; 

  Step1Experiment::Step1Experiment(int size, DatabaseOptions options) {
    size_in_mb = size;
    db = new Database(options);
    std::string db_name = "Step1Experiemnt_" + std::to_string(size) + "MB";
    db->open(db_name);
  }

  void Step1Experiment::putExperiment(){
    ::std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
    for (int i = 0; i < 1; ++i) {
      for(int j = 0; j < 131072 * size_in_mb; ++j) {
        int key_val = i*131072 + j;
        db->put(key_val, key_val);
      }
    }
  ::std::chrono::steady_clock::duration elapsedTime = ::std::chrono::steady_clock::now() - startTime;
  std::cout << std::fixed << std::setprecision(9) << std::endl;
  double duration = ::std::chrono::duration_cast< ::std::chrono::duration< double > >(elapsedTime).count();
  std::cout << "1MB Put Milliseconds: " << duration * 1000 << std::endl;

  }

  void Step1Experiment::closedb() {
    db->close();
  }

};
