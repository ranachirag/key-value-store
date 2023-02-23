#include "Node.h"

Node::Node (long int key, long int value) : key(key), value(value) {
  l = nullptr;
  r = nullptr;
  height = 1;
}