#include "AVL.h"
#include "Node.h"

void AVL_Tree::balance_tree(Node *root_node) {
  // TODO
}

Node *AVL_Tree::insert_node(int key, int value) {
  if (root == nullptr) {
    root = new Node(key, value);;
    return root;
  } else if (key < root->key) {
    root->l = insert_node(key, value);
    // Call Balance Tree

  } else if (key > root->key ) {
    root->r = insert_node(key, value);
    // Call Balance Tree

  }else if (root->key == key) {
    root->value = value;
    return root;
  }
}

void AVL_Tree::delete_node(int key) {
  // TODO
}

Node AVL_Tree::find_node(int key) {
  // TODO
}