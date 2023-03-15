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

TEST_F(AVL_TreeTest, getRootTest) {
  Node *root = tree.get_root();
  ASSERT_EQ(root, nullptr);
  tree.insert(0,0);
  // get root again to update root value
  root = tree.get_root();
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->value, 0);
}

TEST_F(AVL_TreeTest, resetTreeTest) {
  Node *root = tree.get_root();
  ASSERT_EQ(root, nullptr);
  tree.insert(0,0);
  // get root again to update root value
  root = tree.get_root();
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->value, 0);
  // reset tree, get root, and confirm nullptr as root
  tree.reset_tree();
  root = tree.get_root();
  ASSERT_EQ(root, nullptr);
}

TEST_F(AVL_TreeTest, getValueTest) {
  tree.insert(0,0);
  bool val = false;
  Node *root = tree.get_root();
  ASSERT_EQ(tree.get_value(0, val), 0);
}

TEST_F(AVL_TreeTest, insertTest1) {
  tree.insert(0,0);
  ASSERT_EQ(tree.min_key, 0);
  ASSERT_EQ(tree.max_key, 0);
}

TEST_F(AVL_TreeTest, insertTest2) {
  tree.insert(0,0);
  tree.insert(1,1);
  ASSERT_EQ(tree.min_key, 0);
  ASSERT_EQ(tree.max_key, 1);
  Node *root = tree.get_root();
  ASSERT_EQ(root->value, 0);
}

TEST_F(AVL_TreeTest, insertTest3) {
  // rotate left
  tree.insert(0,0);
  tree.insert(1,1);
  tree.insert(2,2);
  ASSERT_EQ(tree.min_key, 0);
  ASSERT_EQ(tree.max_key, 2);
  Node *root = tree.get_root();
  ASSERT_EQ(root->value, 1);
}

TEST_F(AVL_TreeTest, insertTest4) {
  // rotate right
  tree.insert(2,2);
  tree.insert(1,1);
  tree.insert(0,0);
  ASSERT_EQ(tree.min_key, 0);
  ASSERT_EQ(tree.max_key, 2);
  Node *root = tree.get_root();
  ASSERT_EQ(root->value, 1);
}

// TODO test right left rotation

TEST_F(AVL_TreeTest, insertTest5) {
  tree.insert(5,5);
  tree.insert(2,2);
  ASSERT_EQ(tree.min_key, 2);
  ASSERT_EQ(tree.max_key, 5);
  tree.insert(7,7);
  tree.insert(1,1);
  tree.insert(4,4);
  tree.insert(6,6);
  tree.insert(9,9);
  tree.insert(3,3);
  tree.insert(16,16);
  Node *root = tree.get_root();
  ASSERT_EQ(root->r->r->value, 9);
  tree.insert(15, 15);
  ASSERT_EQ(tree.min_key, 1);
  ASSERT_EQ(tree.max_key, 16);
  ASSERT_EQ(root->value, 5);
  ASSERT_EQ(root->r->r->value, 15);
  ASSERT_EQ(root->r->r->r->value, 16);
  ASSERT_EQ(root->r->r->l->value, 9);
}



// TODO test left right rotation

// TODO test print tree

// TODO test range search 
