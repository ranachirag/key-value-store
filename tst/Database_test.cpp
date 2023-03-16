#include <iostream>
#include <algorithm>
#include <list>
#include <limits>

#include "AVL.h"
#include "Node.h"
#include <gtest/gtest.h>

#include "Database.h"
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
  db = new Database(100);
  db->open("testdb");
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
  db = new Database(100);
  db->open("testputget");
  db->put(0,0);
  ASSERT_EQ(db->get(0), 0);
  db->close();
  delete db;
}

TEST_F(Database_test, scanTest) {
  db = new Database(100);
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
