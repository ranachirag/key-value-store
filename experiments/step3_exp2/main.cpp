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
#include "Step3Experiment2.h"
#include "DatabaseMacros.h"
#include <fstream>


int main() {

DatabaseOptions db_options;
db_options.use_buffer_pool = false;
db_options.use_bloom_filters = false;

db_options.memtable_size = NUM_BYTES_MB * 1;

db_options.sst_structure = LIST_SST;
db_options.storage_structure = APPEND_ONLY_STORAGE;

std::cout << "mb_inserted,put_time,get_time,scan_time" << std::endl;
Step1Experiment *exp = new Step3Experiment2(1024, db_options);
exp->run_experiments(1024, 64);


}