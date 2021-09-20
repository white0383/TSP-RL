#include <cmath>
#include "Distance.h"

double dist(Node& n1, Node& n2){
  int dx = n1.x - n2.x;
  int dy = n1.y - n2.y;
  
  return sqrt((double)(dx*dx + dy*dy));
}

int distSquare(Node& n1, Node& n2){
  int dx = n1.x - n2.x;
  int dy = n1.y - n2.y;

  return dx * dx + dy * dy;
}