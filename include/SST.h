#ifndef SST_H
#define SST_H

#include <string>

#include "BufferPool.h"
#include "DatabaseMacros.h"

class SST {
  public: 
    /**
     * Destroy the SST object
     */
    virtual ~SST() {}

    /**
     * Retreive the value for a given key from the SST file
     * Note: Buffer Pool integration supported
     * 
     * @param use_buffer_pool Use a Buffer Pool to check for pages 
     * @param buffer_pool If use_buffer_pool is true, contains the Buffer Pool 
     * @param key Search key
     * @param val_found Set to true if value found
     * @return Value found or -1 if value found
     */
    virtual long search(bool use_buffer_pool, BufferPool *buffer_pool, long key, bool &val_found) = 0;

    /**
     * Scan a range of keys in from the SST file
     * Note: Buffer Pool integration not supported
     * 
     * @param result Stores results from the scan in this variable
     * @param key1 Search key, lower bound
     * @param key2 Search key, upper bound
     * @return Number of items found from scan
     */
    virtual int scan(std::vector<std::pair<long, long> > &result, long key1, long key2) = 0;
    
  protected:
    /**
     * The path to the SST file
     */
    std::string filepath;
};

/**
 * This class represents a SST file that uses the List SST structure
 */
class ListSST : public SST {
  public:
    /**
     * Construct a new List S S T object
     * 
     * @param sst_filepath Filepath to the binary file containing the SST 
     */
    ListSST(std::string sst_filepath);
    ~ListSST() {}
    long search(bool use_buffer_pool, BufferPool *buffer_pool, long key, bool &val_found);
    int scan(std::vector<std::pair<long, long> > &result, long key1, long key2);
};

// TODO: Implement BTree search if we have time
// class BTreeSST : public SST {
//   public:
//     long search(long key);
// };


#endif