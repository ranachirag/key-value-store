#ifndef STEP1EXPERIMENT_H
#define STEP1EXPERIMENT_H

#include "Database.h"
#include "BufferPool.h"

class Step1Experiment{
  public: 
      /**
     * Construct a new PutExperiment object
     * 
     * @param size_in_mb Number of MB of entries to enter into the database
     */
    Step1Experiment(int size_in_mb, DatabaseOptions options);

    int size_in_mb; 

    Database *db;

    /**
     * Add entries to the database and time it 
    */
    std::string putExperiment();

    /**
     * Get entries from database and time it
    */
    std::string getExperiment();

    /**
     * Delete entries from database and time it
    */
    std::pair<std::string, std::string> scanExperiment();

    // /**
    //  * Open Database given the options
    // */
    // void opendb(DatabaseOptions options);

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