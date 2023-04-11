#include <iostream>
#include <algorithm>
#include <list>
#include <limits>

#include "AVL.h"
#include "Node.h"
#include "BufferPool.h"
#include "Database.h"

#include <gtest/gtest.h>
#include <vector>
#include <string>

class Database_test : public ::testing::Test{
  protected:
    Database* db;

    Database_test(){};

    // virtual void SetUp() override {
    //   db = new Database(100);
    //   db->open("testdb");
    //   std::cout << "SetUp called";
    // }

    // virtual void TearDown() override {
    //   std::cout << "TearDown called";
    //   db->close();
    //   delete db;
    // }
};

TEST_F(Database_test, opentest) {
  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5;

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1;
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;

  db = new Database(db_options);

  db->open("testdb");
  // test that if we open another db with the same name that we get an error
  const char* expected_error_message = "Please close the testdb database first";
  testing::internal::CaptureStderr();
  db->open("testdb");
  std::string err_msg = testing::internal::GetCapturedStderr();
  std::string actual_error_message = err_msg.substr(0, err_msg.find(":"));
  EXPECT_EQ(actual_error_message, expected_error_message);
  db->close();
  delete db;
}

TEST_F(Database_test, putGetTest) {
  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5;

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1;
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;

  db = new Database(db_options);

  db->open("testputget");
  db->put(0,0);
  ASSERT_EQ(db->get(0), 0);
  db->close();
  delete db;
}

TEST_F(Database_test, scanTest) {
  BufferPoolOptions buf_options;
  buf_options.initial_size = 1024;
  buf_options.max_size = 4096; // 4096 buckets can hold 4096 frames of 4kB which is 16 MB
  buf_options.max_size_bytes = NUM_BYTES_MB * 10; // 10 MB
  buf_options.evict_policy = CLOCK_EVICT;

  BloomFilterOptions bloom_filter_options;
  bloom_filter_options.parameter_setting = BITS_PER_ENTRY;
  bloom_filter_options.bits_per_entry = 5;

  DatabaseOptions db_options;
  db_options.use_buffer_pool = true;
  db_options.buffer_pool_options = buf_options;
  db_options.memtable_size = NUM_BYTES_MB * 1;
  db_options.use_bloom_filters = true;
  db_options.bloom_filter_options = bloom_filter_options;
  db_options.sst_structure = LIST_SST;
  db_options.storage_structure = LSM_TREE_STORAGE;

  db = new Database(db_options);
  
  db->open("testscan");
  ASSERT_EQ(db->get(0), -1);
  db->put(0,0);
  db->put(1,1);
  db->put(2,2);
  db->put(3,3);
  db->put(5,5);
  std::vector<std::pair<long, long> > lst;
  ASSERT_EQ(db->scan(lst, 1, 1), 1);
  ASSERT_EQ(db->scan(lst, 0, 4), 4);
  ASSERT_EQ(db->scan(lst, 0, 5), 5);    
  db->close();
  delete db;
}
