#include "Graph.h"
#include "Distance.h"
#include <iostream>
#include <algorithm>

using namespace std;

Graph::Graph(vector<vector<int>> &node_data){
  this->n = node_data.size();
  this->nodes.reserve(this->n + 2);

  //add dummy node in nodes[0]
  this->nodes.emplace_back(0,0,0);

  for (auto single_data : node_data) {
    this->nodes.emplace_back(single_data[0], single_data[1], single_data[2]);
  }

  //nodes[0] == nodes[n]
  this->nodes[0].setNode(this->nodes[this->n]);
  //nodes[n+1] == nodes[1]
  this->nodes.emplace_back(this->nodes[1]);

  this->setDistOrder();
}

vector<Node> Graph::getNodes() const{
  return this->nodes;
}

Node Graph::getNode(int i) const{
  return this->nodes.at(i);
}

int Graph::getN() const {
  return this->n;
}

bool compare(pair<int,double> a, pair<int,double> b){
  if (a.second == b.second) {
    return a.first < b.first;
  } else {
    return a.second < b.second;
  }
}

void Graph::setDistOrder() {
  this->distOrder.reserve(this->n +1);

  pair<int,double> dummyPair(0,0.0);
  vector< pair<int,double> > dummyVector(this->n, dummyPair);
  this->distOrder.emplace_back(dummyVector);

  for(int i=1;i<this->n+1;i++){
    dummyVector.clear();
    for(int j=1;j<this->n+1;j++){
      dummyVector.emplace_back(make_pair(j,dist(this->nodes.at(i),this->nodes.at(j))));
    }
    // dummyVector 소트
    sort(dummyVector.begin(), dummyVector.end(), compare);
    this->distOrder.emplace_back(dummyVector);
  }
}