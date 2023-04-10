#ifndef STEP3_EXPERIMENT1_H
#define STEP3_EXPERIMENT1_H

#include "Database.h"
#include "BufferPool.h"

class Step3Experiment1{
  public: 
      /**
     * Construct a new PutExperiment object
     * 
     * @param size_in_mb Number of MB of entries to enter into the database
     */
    Step3Experiment1(int size_in_mb, DatabaseOptions options);

    int size_in_mb; 

    Database *db;

    /**
     * Close database
    */
    void closedb();

    /**
     * Run experiments
     * 
     * @param total_mb Number of MB of entries to enter into the database
     * @param interval_mb Interval to insert and record times
     */
    void run_experiments(int total_mb, int interval_mb);


};

#endif