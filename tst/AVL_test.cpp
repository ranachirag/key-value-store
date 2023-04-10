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

class AVLTreeTest : public ::testing::Test{
  protected:
    AVLTree* tree;

    AVLTreeTest(){};

    virtual void SetUp() override {
      tree = new AVLTree(false);
      std::cout << "SetUp called";
    }

    virtual void TearDown() override {
      std::cout << "TearDown called";
      delete tree;
    }
};

TEST_F(AVLTreeTest, getRootTest) {
  Node *root = tree->get_root();
  ASSERT_EQ(root, nullptr);
  tree->insert(0,0);
  // get root again to update root value
  root = tree->get_root();
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->value, 0);
}

TEST_F(AVLTreeTest, resetTreeTest) {
  Node *root = tree->get_root();
  ASSERT_EQ(root, nullptr);
  tree->insert(0,0);
  // get root again to update root value
  root = tree->get_root();
  ASSERT_NE(root, nullptr);
  ASSERT_EQ(root->value, 0);
  // reset tree, get root, and confirm nullptr as root
  tree->reset_tree();
  root = tree->get_root();
  ASSERT_EQ(root, nullptr);
  ASSERT_NE(tree->min_key, 0);
  ASSERT_NE(tree->max_key, 0);
}

TEST_F(AVLTreeTest, getValueTest) {
  tree->insert(0,0);
  bool val = false;
  Node *root = tree->get_root();
  ASSERT_EQ(tree->get_value(0, val), 0);
}

TEST_F(AVLTreeTest, insertTest1) {
  tree->insert(0,0);
  ASSERT_EQ(tree->min_key, 0);
  ASSERT_EQ(tree->max_key, 0);
}

TEST_F(AVLTreeTest, insertTest2) {
  tree->insert(0,0);
  tree->insert(1,1);
  ASSERT_EQ(tree->min_key, 0);
  ASSERT_EQ(tree->max_key, 1);
  Node *root = tree->get_root();
  ASSERT_EQ(root->value, 0);
}

TEST_F(AVLTreeTest, insertTest3) {
  // rotate left
  tree->insert(0,0);
  tree->insert(1,1);
  tree->insert(2,2);
  ASSERT_EQ(tree->min_key, 0);
  ASSERT_EQ(tree->max_key, 2);
  Node *root = tree->get_root();
  ASSERT_EQ(root->value, 1);
}

TEST_F(AVLTreeTest, insertTest4) {
  // rotate right
  tree->insert(2,2);
  tree->insert(1,1);
  tree->insert(0,0);
  ASSERT_EQ(tree->min_key, 0);
  ASSERT_EQ(tree->max_key, 2);
  Node *root = tree->get_root();
  ASSERT_EQ(root->value, 1);
}

// TODO test right left rotation

TEST_F(AVLTreeTest, insertTest5) {
  ASSERT_EQ(tree->size, 0);
  tree->insert(5,5);
  tree->insert(2,2);
  tree->insert(7,7);
  tree->insert(1,1);
  tree->insert(4,4);
  tree->insert(6,6);
  tree->insert(9,9);
  tree->insert(3,3);
  tree->insert(16,16);
  Node *root = tree->get_root();
  ASSERT_EQ(root->r->r->value, 9);
  tree->insert(15, 15);
  ASSERT_EQ(root->value, 5);
  ASSERT_EQ(root->r->r->value, 15);
  ASSERT_EQ(root->r->r->r->value, 16);
  ASSERT_EQ(root->r->r->l->value, 9);
}

// TODO test left right rotation

TEST_F(AVLTreeTest, insertTest6) {
  tree->insert(5,5);
  tree->insert(2,2);
  tree->insert(7,7);
  tree->insert(1,1);
  tree->insert(4,4);
  tree->insert(6,6);
  tree->insert(9,9);
  tree->insert(3,3);
  tree->insert(16,16);
  Node *root = tree->get_root();
  ASSERT_EQ(root->r->r->value, 9);
  tree->insert(15, 15);
  ASSERT_EQ(root->value, 5);
  ASSERT_EQ(root->r->r->value, 15);
  ASSERT_EQ(root->r->r->r->value, 16);
  ASSERT_EQ(root->r->r->l->value, 9);
}

// TODO test range search 

TEST_F(AVLTreeTest, rangeSearchTest) {
  tree->insert(5,5);
  tree->insert(2,2);
  tree->insert(7,7);
  tree->insert(1,1);
  tree->insert(4,4);
  tree->insert(6,6);
  tree->insert(9,9);
  tree->insert(3,3);
  tree->insert(16,16);
  Node *root = tree->get_root();
  tree->insert(15, 15);
  std::vector<std::pair<long, long> > lst;
  ASSERT_EQ(tree->range_search(lst, 0, 20), 10);
  ASSERT_EQ(tree->range_search(lst, 0, 0), 0); 
  ASSERT_EQ(tree->range_search(lst, 5, 10), 4); 
}