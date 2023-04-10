#include <iostream>
#include <algorithm>
#include <list>
#include <limits>
#include <climits>

#include "AVL.h"
#include "Node.h"

AVLTree::AVLTree(bool updates_supported) : updates_supported(updates_supported){
  root = NULL;
  size = 0;
  min_key = LONG_MAX;
  max_key = LONG_MIN;
}


int get_balance_factor(Node *root_node) {
  int balance_factor = 0;
  if (root_node == NULL){
    return balance_factor;
  } else if (root_node->l != NULL && root_node->r == NULL){
    // left subtree exists but right doesnt so balance is negative left
    balance_factor = -root_node->l->height;
  } else if (root_node->r != NULL && root_node->l == NULL){
    // right subtree exists but left doesnt so balance is right 
    balance_factor = root_node->r->height;
  } else if (root_node->l != NULL && root_node->r != NULL) {
    // right and left subtrees exist,  so balance factor is right - left
    balance_factor = root_node->r->height - root_node->l->height;
  } 
  return balance_factor;
}

long get_height(Node *root_node){
  if (root_node == NULL) {
    return 0;
  }
  if (root_node->l != NULL && root_node->r != NULL){
    root_node->height = std::max(root_node->l->height, root_node->r->height) + 1;
  } else if (root_node->l != NULL && root_node->r == NULL){
    root_node->height = root_node->l->height + 1;
  } else if (root_node->l == NULL && root_node->r != NULL){
    root_node->height = root_node->r->height + 1;
  } else {
    root_node->height = 1;
  }
  return root_node->height;
}

Node *rotate_left(Node *root_node){

  Node *old_right = root_node->r;
  Node *old_right_left = old_right->l;

  old_right->l = root_node;
  root_node->r = old_right_left;

  root_node->height = get_height(root_node);
  old_right->height = get_height(old_right);

  return old_right;
}


Node *rotate_right(Node *root_node){
  Node *old_left = root_node->l;
  Node *old_left_right = old_left->r;

  old_left->r = root_node; 
  root_node->l = old_left_right;

  root_node->height = get_height(root_node);
  old_left->height = get_height(old_left);

  return old_left;
}


Node *AVLTree::balance_tree(Node *root_node, long key) {
  int balance_factor = get_balance_factor(root_node);
  if (balance_factor >= 2){
    if (key > root_node->r->key){
      // Rotate left
      return rotate_left(root_node);
    } else if (key < root_node->r->key){
      // Rotate right then left
      root_node->r = rotate_right(root_node->r);
      return rotate_left(root_node);
    }
  } else if (balance_factor <= -2){
    if (key < root_node->l->key){
      return rotate_right(root_node);
    } else if (key > root_node->l->key){
      // Rotate left then right
      root_node->l = rotate_left(root_node->l);
      return rotate_right(root_node);
    }
  }
  return root_node;
}


Node *AVLTree::insert_node(Node *root_node, long key, long value) {
  if (root_node == NULL) {
    root_node = new Node(key, value);
    return root_node;
  } else if (root_node->key == key) {
    if(updates_supported){
      root_node->value = value;
    }
    return root_node;
  } else if (key < root_node->key) {
    root_node->l = insert_node(root_node->l, key, value);
  } else if (key > root_node->key) {
    root_node->r = insert_node(root_node->r, key, value);
  }
  root_node->height = get_height(root_node);
  root_node = balance_tree(root_node, key);
  
  return root_node;
}

Node *AVLTree::find_node(Node *root_node, long key) {
  if(root_node == NULL){
    return NULL;
  } else if (key == root_node->key) {
    return root_node;
  } else if (key < root_node->key) {
    if (root_node->l != NULL){
      return find_node(root_node->l, key);
    }
  } else if (key > root_node->key) {
    if (root_node->r != NULL){
      return find_node(root_node->r, key);
    }
  }
  return NULL;
}

void AVLTree::reset_tree_nodes(Node *&root_node) {
  if(!root_node) {
    return;
  } 
  reset_tree_nodes(root_node->l);
  reset_tree_nodes(root_node->r);
  delete root_node;
  root_node = nullptr;
}


void AVLTree::insert(long key, long value) {
  root = insert_node(root, key, value);
  size += VALUE_SIZE;
  if (key < min_key) {
    min_key = key;
  }
  if (key > max_key) {
    max_key = key;
  }
}


long AVLTree::get_value(long key, bool &val_found) {
  Node *node = find_node(root, key);
  val_found = false;
  if (node == NULL ) {
    return -1;
  }
  val_found = true;
  return node->value;
}


int AVLTree::range_search_nodes(std::vector<std::pair<long, long> > &result, Node *root_node, long key1, long key2) {
  if (root_node == NULL) {
    return 0;
  }

  if (root_node->key > key2) {
    // search left subtree only
    range_search_nodes(result, root_node->l, key1, key2);
  } 
  if (root_node->key < key1) {
    // search right subtree only
    range_search_nodes(result, root_node->r, key1, key2);
  } 
  
  if (root_node->key >= key1 && root_node->key <= key2) {
    // search both subtrees

    int l_results = range_search_nodes(result, root_node->l, key1, root_node->key);
    
    std::pair<long, long> root_pair (root_node->key, root_node->value);

    result.push_back(root_pair);    

    int r_results = range_search_nodes(result, root_node->r, root_node->key, key2);

  }
  return result.size();
}

int AVLTree::range_search(std::vector<std::pair<long, long> > &result, long key1, long key2) {
  int prev_size = result.size();
  return range_search_nodes(result, root, key1, key2) - prev_size;
}

void AVLTree::reset_tree() {
  reset_tree_nodes(root);
  size = 0;
  min_key = LONG_MAX;
  max_key = LONG_MIN;
}

Node *AVLTree::get_root() {
  return root;
}


