
#include <gtest/gtest.h>
#include <cmath>
#include "BufferPool.h"

class BufferPoolTest : public ::testing::Test{
  protected:
    BufferPool* buffer_pool;

    BufferPoolTest(){};

    virtual void SetUp() override {
      int initial_size = pow(2, 3);
      int max_size = pow(2, 4);
      buffer_pool = new BufferPool(initial_size, max_size);
      std::cout << "SetUp called";
    }

    virtual void TearDown() override {
      std::cout << "TearDown called";
      delete buffer_pool;
    }
};

TEST_F(BufferPoolTest, InitializationTest) {
  
}

