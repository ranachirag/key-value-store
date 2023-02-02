#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "Node.h"

class AVL_Tree {
  private:
    void balance_tree(Node *root_node);
  public:
    Node *root;
    Node *insert_node(int key, int value);
    void delete_node(int key);
    Node find_node(int key); // Return value only instead of Node?
};

#endif