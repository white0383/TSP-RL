#include "Node.h"
#include <iostream>

using namespace std;

Node::Node(int index, int x, int y) {
  this->index = index;
  this->x = x;
  this->y = y;
}

void Node::printNode() {
  cout << "index : " << this->index << "\t" ;
  cout << "x : " << this->x << "\t";
  cout << "y : " << this->y << endl;
}