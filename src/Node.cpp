#include "Node.h"

Node::Node (int key, int value) : key(key), value(value) {
  l = nullptr;
  r = nullptr;
  balance_factor = 0;
}