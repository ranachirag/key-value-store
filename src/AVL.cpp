#include "AVL.h"
#include "Node.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <limits>

AVL_Tree::AVL_Tree() {
  size = 0;
  min_key = std::numeric_limits<long int>::infinity();
  max_key = -std::numeric_limits<long int>::infinity();
}

// TODO: Remove, only used for debugging 
void printTree(Node *root_node, std::string indent, bool last) {
  if (root_node != NULL) {
    std::cout << indent;
    if (last) {
      std::cout << "R----";
      indent += "   ";
    } else {
      std::cout << "L----";
      indent += "|  ";
    }
    std::cout << root_node->key << " (H = " << root_node->height << ")" << std::endl;
    printTree(root_node->l, indent, false);
    printTree(root_node->r, indent, true);
  }
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

int get_height(Node *root_node){
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


Node *AVL_Tree::balance_tree(Node *root_node, long int key) {
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


Node *AVL_Tree::insert_node(Node *root_node, long int key, long int value) {
  if (root_node == NULL) {
    root_node = new Node(key, value);
    return root_node;
  } else if (root_node->key == key) {
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

Node *AVL_Tree::find_node(Node *root_node, long int key) {
  if (key == root_node->key) {
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


void AVL_Tree::insert(long int key, long int value) {
  root = insert_node(root, key, value);
  size += VALUE_SIZE;
  if (key < min_key) {
    min_key = key;
  }
  if (key > max_key) {
    max_key = key;
  }
}


int AVL_Tree::get_value(long int key) {
  Node *node = find_node(root, key);
  if (node == NULL) {
    return -1;
  }
  return node->value;
}

void AVL_Tree::print_tree() {
  printTree(root, "", false);
}

std::list<std::pair<long int,long int>> AVL_Tree::range_search_nodes(Node *root_node, long int key1, long int key2) {
  std::list<std::pair<long int, long int>> pairs;
  if (root_node == NULL) {
    return pairs;
  }

  if (root_node->key > key2) {
    // search left subtree only
    pairs = range_search_nodes(root_node->l, key1, key2);
  } 
  if (root_node->key < key1) {
    // search right subtree only
    pairs = range_search_nodes(root_node->r, key1, key2);
  } 
  
  if (root_node->key >= key1 && root_node->key <= key2) {
    // search both subtrees

    std::list<std::pair<long int, long int>> l_pairs = range_search_nodes(root_node->l, key1, root_node->key);
    std::list<std::pair<long int, long int>> r_pairs = range_search_nodes(root_node->r, root_node->key, key2);

    pairs = l_pairs;
    std::pair<long int, long int> root_pair{root_node->key, root_node->value};
    pairs.push_back(root_pair);
    pairs.merge(r_pairs);

  }
  return pairs;
}

std::list<std::pair<long int, long int>> AVL_Tree::range_search(long int key1, long int key2) {
  return range_search_nodes(root, key1, key2);
}

