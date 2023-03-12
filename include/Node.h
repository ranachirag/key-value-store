#ifndef NODE_H
#define NODE_H

/**
 * This class represents a Node in an AVL Tree
*/
class Node {
  public:
    /**
     * Constructor
     * 
     * @param key Key to store in node
     * @param value Value to store in node
    */
    Node (long key, long value);

    /**
     * Key (AVL Tree sorts nodes by this value)
    */
    long key;

    /**
     * Value (the data part of the node)
    */
    long value;

    /**
     * The height in the AVL Tree the node is at
    */
    long height;

    /**
     * Left child node
    */
    Node *l;

    /**
     * Right child node
    */
    Node *r;
};

#endif