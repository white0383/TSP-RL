#ifndef TSP_TOUR_H
#define TSP_TOUR_H

#include <vector>
#include "Graph.h"
#include "Distance.h"

/**
 * Tour and Graph are implemented independently
 * you should use them together when calculate something
 * 
 * Not only a tour which has every city of TSP,
 * but also a subtour which partly has some cities of TSP
 * are Tour object
 * 
 * Example)
 * Graph = {1,2,3,4}  (index of nodes)
 * Tour pi_1 = {1,3,2,4,1} -> this is complete tour
 * Tour pi_2 = {2,4,3,2} -> this is not complete tour
 * Tour pi_3 = {1,3,2} -> this is not Tour
 */

class Tour{
  private:
    /**
     * The order of visiting city
     * integer in the vector represent the index of city.
     * IMPORTANT : city of index i stored in nodes[i-1]
     */
    vector<int> tour;

    /**
     * Total distance of Tour
     * Sum of every edge on Tour
     */
    double cost;

    /**
     * The number of cities which Tour object has.
     * NOT the size of tour vector
     */
    int size;

  public:
    /**
     * Default constructor
     * set cost to DBL_MAX
     * size to zero
     */
    Tour();

    /**
     * Create new Tour object with order vector
     * 
     * Example)
     * Tour t1 = new Tour({1,4,2})
     * t1.tour -> {1,4,2,1}
     */
    Tour(const vector<int> &order, const Graph& g);

    /**
     * Default destructor
     */
    ~Tour() = default;

    /**
     * CompleteTour is "Verified" Tour which has g.getNodesSize()+1 elements in tour
     * 
     * @param Graph g
     * @return true -> this object is complete tour
     */
    bool isCompleteTour(const Graph& g);

    /**
     * Get Tour's cost
     * 
     * @return this->cost
     */
    double getCost();

    /**
     * Calculate Tour's cost
     * 
     * @param Graph g
     */
    void setCost(const Graph &g);

    /**
     * Get tour vector of this object
     * 
     * @return this->tour 
     */
    vector<int>& getTour();

    /**
     * Get size of this tour object
     * 
     * @return this->size
     */
    int getSize();

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
    void printTour();
};

/**
 * Check tour vector is well-defined.
 * If there is any node that apeears more than once in tour vector,
 * or if there is any node that is not included in Graph g,
 * then this tour vector is not well defined
 * 
 * @param 
 * @return true -> this vector is good Tour
 */
bool verifyTour(const vector<int> &tour, const Graph& g);

#endif //TSP_TOUR_H