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

class AVL_TreeTest : public ::testing::Test{
  protected:
    AVL_Tree tree;
};

TEST_F(AVL_TreeTest, insertTest1) {
  
  tree.insert(0,0);
  
  EXPECT_EQ(tree.min_key, 0);
  EXPECT_EQ(tree.max_key, 0);
}