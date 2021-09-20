#ifndef TSP_SEARCHLOCALOPT_H
#define TSP_SEARCHLOCALOPT_H

#include <string>
#include "../../model/Tour.h"
#include "../../model/Graph.h"

/**
 * Return local optimal solution in the neighborhood of given Tour pi.
 * 
 * @param Graph g : TSP itself
 * @param string method : the name of local-opt search method
 * @param Tour pi: initial solution of TSP 
 * @return Tour : Local opt in the neighborhood of "solution"
 */
Tour searchLocalOpt(const Graph& g, const string& method, Tour& pi);

#endif //TSP_SEARCHLOCALOPT_H