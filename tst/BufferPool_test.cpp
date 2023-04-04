
#include <gtest/gtest.h>
#include <cmath>
#include "BufferPool.h"

class BufferPoolTest : public ::testing::Test{
  protected:
    BufferPool* buffer_pool;

    BufferPoolTest(){};

    virtual void SetUp() override {
      BufferPoolOptions buf_options;
      buf_options.initial_size = pow(2, 3);
      buf_options.max_size = pow(2, 3);
      std::string policy = "CLOCK";
      buffer_pool = new BufferPool(buf_options);
      std::cout << "SetUp called";
    }

    virtual void TearDown() override {
      std::cout << "TearDown called";
      delete buffer_pool;
    }
};

TEST_F(BufferPoolTest, InitializationTest) {
  
}

