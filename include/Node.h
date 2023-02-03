#ifndef NODE_H
#define NODE_H

class Node {
  public:
    Node (int key, int value);
    int key;
    int value;
    int balance_factor;
    int height;
    Node *parent;
    Node *l;
    Node *r;
};

#endif