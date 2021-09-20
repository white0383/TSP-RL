#include "Graph.h"
#include <iostream>

using namespace std;

Graph::Graph(vector<vector<int>> &node_data){
  this->nodes.reserve(node_data.size());

  for (auto single_data : node_data) {
    this->nodes.emplace_back(single_data[0], single_data[1], single_data[2]);
  }
  /*
  int nodeDataSize = node_data.size();
  int index = 0;
  int x = 0;
  int y = 0;

  for(int i=0; i<nodeDataSize; i++){
    index = node_data[i][0];
    x = node_data[i][1];
    y = node_data[i][2];
    this->nodes.push_back(new Node(index,x,y));
  }
  */

  /*
  this->nodes.reserve(nodeDataSize);

  for(int i=0; i<nodeDataSize; i++){
    index = node_data[i][0];
    x = node_data[i][1];
    y = node_data[i][2];
    this->nodes[i] = 
    nodes[i] = new Node(index, x, y);
  }
  */
}

vector<Node>& Graph::getNodes() {
  return this->nodes;
}

int Graph::getNodesSize() const {
  return this->nodes.size();
}

void Graph::setNodes(vector<Node>& nodes){
  if (this->getNodesSize() != 0) {
    cout << "Error : Set Nodes in not-empty graph is not permitted" << endl;
    cout << "Terminate program" << endl;
    exit(1);
  }
  this->nodes.assign(nodes.begin(), nodes.end());
}