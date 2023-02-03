#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "Node.h"

class AVL_Tree {
  private:
    void balance_tree(Node *root_node);
    int get_height(Node *root_node); // get height of node
    int get_balance_factor(Node *root_node); // get balance factor of gicven node
  public:
    Node *root;
    Node *insert_node(int key, int value);
    void delete_node(int key);
    Node find_node(int key); // Return value only instead of Node?
};

#endif