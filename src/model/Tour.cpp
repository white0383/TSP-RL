#include "Tour.h"
#include "Graph.h"
#include "Distance.h"
#include <vector>
#include <iostream>
#include <cfloat> // DBL_MAX

using namespace std;

Tour::Tour(){
  this->cost = DBL_MAX;
  this->size = 0;
}

Tour::Tour(const vector<int> &order, const Graph& g){
  vector<int> tmpTourVector = order;
  tmpTourVector.push_back(tmpTourVector[0]); //connect head and tail

  if(verifyTour(order, g) != true){
    cout << "WARNING : improper input vector in Tour object constructor";
  }else{
    this->tour.assign(tmpTourVector.begin(), tmpTourVector.end());
    this->cost = DBL_MAX;
    this->size = order.size();
  }
}

bool Tour::isCompleteTour(const Graph& g){
  bool isWellDefindeCompleteTour = true;

  vector<int> order = this->tour;
  order.pop_back(); // cut its tail

  if(!(verifyTour(order, g))){
    cout << "WARNING : this is not complete tour (this is not a tour)" << endl;
    isWellDefindeCompleteTour = false;
  } else if(this->getSize() != g.getNodesSize()){
    cout << "WARNING : this is not complete tour (does not visit every cities)" << endl;
    isWellDefindeCompleteTour = false;
  }

  return isWellDefindeCompleteTour;
};

double Tour::getCost() {
  return this->cost;
};

void Tour::setCost(const Graph &g) {
  double cost = 0;

  vector<int>::iterator it;
  for(it=this->tour.begin(); it!=this->tour.end() - 1 ;it++){
    /**
     * *it -> current node's index
     * (*it)-1 -> the node whose index is *it is stored in nodes[(*it)-1] in g because "nodes" 's index starts with 0
     * 
     * *(it+1) -> next node's index 
     */
    Node currentNode = g.nodes[(*it)-1];
    Node nextNode = g.nodes[(*(it + 1)) - 1];
    double length = dist(currentNode, nextNode);
    cost += length;
  }

  this->cost = cost;
};

vector<int>& Tour::getTour() {
  return this->tour;
};

int Tour::getSize() {
  return this->size;
}

    /**
     * Print tour and cost.
     * If cost is not yet calculated (i.e. this->cost == DBL_MAX),
     * then print cost as "Not Calculated"
     * 
     * Example)
     * tour = {1,3,2,4,1}
     * cost = 102.4
     * 
     * then printTour() is
     * "tour : 1 -> 3 -> 2 -> 4 -> 1
     * cost : 102.4"
     */
void Tour::printTour() {
  //1. print tour
  cout << "tour : " ;

  vector<int>::iterator it;
  for(it=this->tour.begin(); it!=this->tour.end() - 1 ;it++){
    cout << *it << " -> ";
  }
  cout << this->tour[0] << endl;

  //2. print cost
  cout << "cost : " ;
  if(this->cost == DBL_MAX){
    cout << "Not yet Calculated" << endl;
  } else {
    cout << this->cost << endl;
  }
}

bool verifyTour(const vector<int> &order, const Graph& g){
  int maxIndex = g.getNodesSize();
  vector<bool> visited = vector(maxIndex+1,false);
  bool isWellDefined = true;

  for(int city : order){

    if((city > maxIndex) || (visited[city] == true)){
      isWellDefined = false;
      break;
    }else{
      visited[city] = true;
    }
  }

  return isWellDefined;
}