#ifndef NODE_H
#define NODE_H

class Node {
  public:
    Node (long key, long value);
    long key;
    long value;
    long height;
    Node *l;
    Node *r;
};

#endif