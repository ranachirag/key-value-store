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

int main() {

  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 1024;

  DatabaseOptions db_options;
  db_options.use_buffer_pool = false;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = 4096;

  int sizes[11] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
  for (int size: sizes){
    Step1Experiment *exp = new Step1Experiment(size, db_options);
    exp->putExperiment();
    exp->getExperiment();
    exp->scanExperiment();
    // exp->deleteExperiment();
  }

}