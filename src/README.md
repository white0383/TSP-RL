# Description of each directory

  1. model : Objects of TSP
    * Node -> information of each cities
    * Graph -> set of Nodes, TSP itself
    * Tour -> child of Graph class. The order of nodes has meaning
    * Distance -> function to calculate distance (Node, Node), (Tour, Node)
  2. helper : supplementary classes and functions
    * FileIOHelper -> functions to read(or write) extenal data file
    * Timer -> class to record running time
  3. solver : functions which map Graph to Tour
    * /tool -> functions which mutually used in solvers
    * /initial_solution -> initial solution generator and its methods
    * /local_search -> local searcher and its methods
    * /exact_solution -> for concorde solver
  4. perturber : functions which perturb Tour to another Tour