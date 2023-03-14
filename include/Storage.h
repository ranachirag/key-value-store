#ifndef STORAGE_H
#define STORAGE_H

#include <algorithm>
#include <vector>
#include <string>

#include "SST.h"

/**
 * This class represents an interface for SST storage
*/
class Storage {
  public:
    /**
     * Constructor for empty SST storage
     * 
     * @param db_name Name of database to store SSTs
    */
    Storage(std::string db_name);

    /**
     * Constructor given an intial set of SSTs (sorted by oldest to youngest)
     * 
     * @param db_name Name of database to store SSTs
     * @param sst_files SST filenames
    */
    Storage(std::string db_name, std::vector<std::string> sst_files);

    /**
     * Add a list of key-value pairs to SST storage
     * 
     * @param data Key-value pairs (sorted by keys)
    */
    void add_to_storage(std::vector<std::pair<long, long> > &data);

    /**
     * Retreive a value with a given associated key from SST storage
     * 
     * @param key Key to search
     * @param val_found Indicates if the value was found
     * 
     * @return Value found from searching SST storage with given key
    */
    long get_value(long key, bool &val_found);

    /**
     * Retreive all key-value pairs with a given key range from SST storage
     * 
     * @param result Results of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs found within given key range
    */
    int scan_storage(std::vector<std::pair<long, long> > &result, long key1, long key2);

    /**
     * Reset Storage object
     */
    void reset();

  private:
    /**
     * Calculates the number of SST files in SST storage
     * 
     * @return Number of SST files in SST storage
    */
    int num_SST();

    /**
     * Get the filename of the next (youngest) SST to insert 
     * 
     * Note: Structure of SST files are: `SST_{NUM}.bin` where NUM is an integer value representing the
     *       age of SST file. For example, if there are 10 existing SST files in storage, the oldest file
     *       would be `SST_0.bin` and the youngest existing file would be `SST_9.bin` and this function 
     *       would return the string value `SST_10.bin` representing the new youngest file to insert into
     *       storage.
     *
     * @return Filename of the youngest SST 
    */
    std::string get_SST_filename(); 

    /**
     * Construct path for a SST file
     * 
     * @param sst_filename SST filename
     * 
     * @return Path to the SST file from project root directory
    */
    std::string get_SST_filepath(std::string sst_filename);

    /**
     * Create a SST object for an existing SST binary file
     * 
     * @param filename Name of SST file to create object for
    */
    void create_SST(std::string filename);

    /**
     * The name of the database associated with current SST storage
    */
    std::string db_name; 

    /**
     * List of object representations of all the SSTs in storage
    */
    std::vector<SST*> SSTs;

};

#endif