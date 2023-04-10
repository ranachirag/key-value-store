#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <algorithm>
#include <vector>

#include "Node.h"
#include "DatabaseMacros.h"


/**
 * This class represents an AVL Tree
 */
class AVLTree {
  public:
    /**
     * Constructor
     * 
     * @param updates_supported Support updates in AVL Tree
    */
    AVLTree(bool updates_supported);

    /**
     * Insert a key-value pair
     * 
     * @param key Key
     * @param value Value
    */
    void insert(long key, long value);

    /**
     * Retrieve a value from the tree given the associated key
     * 
     * @param key Key
     * @param val_found Indicates if the value was found
     * 
     * @return Value 
    */
    long get_value(long key, bool & val_found);

    /**
     * Retrieve all key-value pairs from a given range of keys
     * 
     * @param result Result of range scan will be stored in this vector
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of key-value pairs found in range scan
    */
    int range_search(std::vector<std::pair<long, long> > &result, long key1, long key2);

    /**
     * Reset the tree such that it is empty
    */
    void reset_tree();

    /**
     * Size of the tree, in terms of the number of bytes of all Values it contains
    */
    long size;

    /**
     * Smallest key in the tree currently
    */
    long min_key;

    /**
     * Largest key in the tree currently
    */
    long max_key;

    /** 
     * Get root node
    */
    Node *get_root();

  private:
    
    /**
     * Supports updates in AVL Tree
     */
    bool updates_supported;

    /**
     * The root node of the AVL Tree
    */
    Node *root;

    /**
     * Balance the AVL Tree
     * 
     * @param root_node Root node of the AVL tree
     * @param key Inserted key causing imbalance 
     * 
     * @return Root node of balanced AVL tree
    */
    Node *balance_tree(Node *root_node, long key);

    /**
     * Insert a node in the AVL Tree, recursive
     * 
     * @param root_node Root node of the AVL tree
     * @param key Key to insert
     * @param value Value to insert
     * 
     * @return Root node of AVL Tree containing inserted node
    */
    Node *insert_node(Node *root_node, long key, long value);

    /**
     * Retreive node with given key from tree, recursive
     * 
     * @param root_node Root node of the AVL tree
     * @param key Key of node to find
     * 
     * @return Root node that was found, nullptr otherwise
    */
    Node *find_node(Node *root_node, long key);
    
    /**
     * Retreive all nodes with given key range from tree, recursive
     * 
     * @param root_node Root node of the AVL tree
     * @param key1 Start key of range
     * @param key2 End key of range
     * 
     * @return Number of nodes found in the given range
    */
    int range_search_nodes(std::vector<std::pair<long, long> > &result, Node *root_node, long key1, long key2);

    /**
     * Delete all nodes in the tree, recursive
     * 
     * @param root_node Root node of the AVL tree
     * @param key Key of node to find
    */
    void reset_tree_nodes(Node *&root_node);
};

#endif