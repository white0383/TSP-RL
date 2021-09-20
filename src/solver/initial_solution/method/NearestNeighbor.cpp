#include <cstdlib> // should be replaced to <random>'s mt19937
#include <iostream>
#include <vector>

#include "NearestNeighbor.h"
#include "../../../model/Tour.h"
#include "../../../model/Graph.h"

using namespace std;

Tour nearestNeighbor(const Graph& g){
  vector<int> order;

  for(int i=0;i<10;i++){
    int tmp = rand() % 100;
    cout << "random number : " << tmp << endl; 
  }

  Tour tmpTour;

  return tmpTour;
}