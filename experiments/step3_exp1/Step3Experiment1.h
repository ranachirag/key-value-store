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

};

#endif