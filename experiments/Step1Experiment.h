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
    void putExperiment();

    /**
     * Get entries from database and time it
    */
    void getExperiment();

    /**
     * Delete entries from database and time it
    */
    void deleteExperiment();

    // /**
    //  * Open Database given the options
    // */
    // void opendb(DatabaseOptions options);

    /**
     * Close database
    */
    void closedb();

};

#endif