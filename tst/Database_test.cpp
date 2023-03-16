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

    virtual void SetUp() override {
      db = new Database(100);
      db->open("testdb");
      std::cout << "SetUp called";
    }

    virtual void TearDown() override {
      std::cout << "TearDown called";
      db->close();
      delete db;
    }
};

TEST_F(Database_test, getOpentest) {
  const char* expected_error_message = "Please close the testdb database first";
  testing::internal::CaptureStderr();
  db->open("testdb");
  std::string err_msg = testing::internal::GetCapturedStderr();
  std::string actual_error_message = err_msg.substr(0, err_msg.find(":"));
  EXPECT_EQ(actual_error_message, expected_error_message);
}