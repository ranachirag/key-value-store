#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <algorithm>
#include <list>
#include "Node.h"

#define KEY_SIZE 4
#define VALUE_SIZE 4

class AVL_Tree {
  private:
    Node *root;
    Node *balance_tree(Node *root_node, long int key);
    Node *insert_node(Node *root_node, long int key, long int value);
    Node *find_node(Node *root_node, long int key);
    std::list<std::pair<long int, long int>> range_search_nodes(Node *root_node, long int key1, long int key2);
  public:
    AVL_Tree();
    int size;
    long int min_key;
    long int max_key;
    void insert(long int key, long int value);
    int get_value(long int key);
    std::list<std::pair<long int, long int>> range_search(long int key1, long int key2);
    void print_tree();
};

#endif