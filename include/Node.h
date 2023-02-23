#ifndef NODE_H
#define NODE_H

class Node {
  public:
    Node (long int key, long int value);
    long int key;
    long int value;
    int height;
    Node *l;
    Node *r;
};

#endif