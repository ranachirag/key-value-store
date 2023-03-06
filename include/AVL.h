#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "Node.h"

#define KEY_SIZE 8
#define VALUE_SIZE 8

class AVL_Tree {
  private:
    Node *root;
    Node *balance_tree(Node *root_node, long key);
    Node *insert_node(Node *root_node, long key, long value);
    Node *find_node(Node *root_node, long key);
    void reset_tree_nodes(Node *&root_node);
    std::vector<std::pair<long, long>> range_search_nodes(Node *root_node, long key1, long key2);
  public:
    AVL_Tree();
    long size;
    long min_key;
    long max_key;
    void insert(long key, long value);
    long get_value(long key);
    std::vector<std::pair<long, long>> range_search(long key1, long key2);
    void reset_tree();
    void print_tree();
};

#endif