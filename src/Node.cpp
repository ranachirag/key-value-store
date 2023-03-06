#include "Node.h"

Node::Node (long key, long value) : key(key), value(value) {
  l = nullptr;
  r = nullptr;
  height = 1;
}