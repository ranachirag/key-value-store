#include "AVL.h"
#include "Node.h"
#include <iostream>
#include <algorithm>

void AVL_Tree::balance_tree(Node *root_node) {
  // TODO

  if (root->balance_factor == 2){
    if (root->r->balance_factor == 1){
      // Rotate left
      Node *old_root;
      Node *old_right;
      old_root = root;
      old_right = root->r;

      old_root->r = old_right->l;
      old_right->l = old_root;

      root = old_right;

    } else if (root->l->balance_factor == -1){
      // Rotate right then left
    }
  } else if (root->balance_factor == -2){
    if (root->l->balance_factor == -1){
      // Rotate right
      Node *old_root;
      Node *old_left;
      old_root = root;
      old_left = root->l;

      old_root->l = old_left->r;
      old_left->r = old_root;

      root = old_left;

    } else if (root->l->balance_factor == 1){
      // Rotate left then right
    }
  }

}


int AVL_Tree::get_height(Node *root_node){
  // TODO
  if (root_node->l && root_node->l){
    root_node->height = std::max(root_node->l->height, root_node->r->height) + 1;
  } else if (root_node->l && root_node->r == nullptr){
    root_node->height = root_node->l->height + 1;
  } else {
    root_node->height = root_node->r->height + 1;
  }
}



// left tree is negative, left tree is positive
Node *AVL_Tree::insert_node(int key, int value) {
  if (root == nullptr) {
    root = new Node(key, value);
    root->height = 1;
    return root;

  } else if (root->key == key) {
    root->value = value;
    return root;

  } else if (key < root->key) {
    root->l = insert_node(key, value);
    root->l->parent = root;
    root->balance_factor = get_balance_factor(root);


  } else if (key > root->key) {
    root->r = insert_node(key, value);
    root->r->parent = root;
    root->balance_factor = get_balance_factor(root);

  }
  root->height = get_height(root);
  balance_tree(root);

}

void AVL_Tree::delete_node(int key) {
  // TODO
}

Node AVL_Tree::find_node(int key) {
  // TODO
}

int  AVL_Tree::get_balance_factor(Node *root_node) {
  // TODO
  if (root->l != nullptr && root->r == nullptr){
    // left subtree exists but right doesnt so balance is negative left
    root->balance_factor = -root->l->height;
  } else if (root->r != nullptr && root->l == nullptr){
    // right subtree exists but left doesnt so balance is right 
    root->balance_factor = root->r->height;
  } else if (root->l != nullptr && root->r != nullptr) {
    // right and left subtrees exist,  so balance factor is right - left
    root->balance_factor = root->r->height - root->l->height;
  } else {
    root->balance_factor = 0;
  }
  return root->balance_factor;
}