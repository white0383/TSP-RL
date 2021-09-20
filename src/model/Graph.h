#ifndef TSP_GRAPH_H
#define TSP_GRAPH_H

#include <vector>
#include "Node.h"

using namespace std;

class Graph {
  public:
    /**
     * Node set of the graph
     */
    vector<Node> nodes;

    /**
     */
  
  public:
    /**
     * Default Graph Constructor
     * Create an empty Graph object
     */
    Graph() = default;

    /**
     * Graph Constructor
     * Create Graph object with node data.
     * 
     * @param |V| * 3 int 2D array node_data
     * Memo: it should be changed(or overroaded) when double type coordinate needed(Delete me when it resolved. written in  2021-09-14 )
     */
    Graph(vector<vector<int>> &node_data);

    /**
     * Return the node set of this graph.
     * 
     */
    vector<Node>& getNodes();

    /**
     * Return the number of nodes that this graph.
     * 
     * @return the size of "nodes" vector
     */
    int getNodesSize() const;

    /**
     * Set Graph object's nodes data manually
     * Only operate when Graph object is empty
     * Exception check required
     * 
     * @param vector of Node 
     */
    void setNodes(vector<Node>& nodes);
};

#endif //TSP_GRAPH_H