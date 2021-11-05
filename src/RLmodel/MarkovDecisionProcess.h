#ifndef TSP_MDP_H
#define TSP_MDP_H

#include <vector>
#include <utility> //std::pair
#include "../model/Tour.h"
#include "../model/Arguments.h"
#include "./ReinLearnMemory.h"

using namespace std;

//predeclaration
class State;
class Action;
class MDP;

namespace StateHelper{
  /**
   * initialization of vector pi_vec and pi_inv_vec
   * Convert pi_tour to 2 vectors
   * pi_vec and pi_inv_vec have to be declared before use this
   * They are cleared to empty and redifined by this
   * This mainly used in perturbation
   * 
   * description of arguments)
   * 1. Tour& pi_tour : complete tour of TSP
   * 2. pi_vec : map order to node
   * 3. pi_inv_vec : map node to order
   * 
   * example)
   * Tour.tour = {1,4,2,3}, n=4
   * then, 
   * pi_vec = {3,1,4,2,3,1} 
   * pi_vec[0] is pi_vec[n]
   * pi_vec[n+1] is pi_vec[1]
   * pi_inv_vec = {0,1,3,4,2}
   * pi_inv_vec[0] is always 0 (dummy data)
   * pi_vec.at(pi_inv_vec.at(i)) == i 
   * pi_inv_vec.at(pi_vec.at(j)) == j
   * 
   * In Tour class, 
   * method called .pi and .piInverse are already implemented
   * but .piInverse is slow ( O(n) )
   * 
   * In perturbation, 
   * Tour of TSP is handled as pi_vec and pi_inv_vec internaly
   * Like 2OPT and F2OPT, pi_vec finally re-converted to Tour
   * In that time, pi_vec[0] and pi_vec[n+1] have to be cut
   */
  void initPiAndPiInv(Tour& pi_tour, vector<int>& pi_vec, vector<int>& pi_inv_vec);

  void eraseDummiesInPiVec(vector<int>& pi_vec);
}

namespace ActionHelper{
  /**
   * encode a pair (a,b) to int x
   * n is maximum value of b (i.e. the number of cities)
   * a < b is always satisfied
   * x is between 1 and (n*n - n) / 2 (i.e. summation of from 1 to n-1)
   * 
   * example) 
   * (1,2) -> 1 , (1,3) -> 2, ... , (1,n) -> n-1, 
   * (2,3) -> (n-1) + 1, (2,4) -> (n-1) + 2 , ... ,
   * (n-2, n-1) -> (n-1) + (n-2) + ... + (3) + 1 ,
   * (n-2, n) -> (n-2, n-1) + 1,
   * (n-1, n) -> (n*n - n) / 2
   * 
   * so, f((a,b),n) = sum_(i=1 to a-1){n-i} + (b-a)
   *                = (a-1)*n - (a*a - a)/2 + (b-a)
   */
  int encodeSwap(pair<int,int> swap, int n);

  /**
   * inverse Function of encodeSwap
   * swap and swapCode are bijective
   * 
   * I don't know how to implement it O(1)
   * so I implement it by using iteration
   * 
   * x((a,b),n) is sum_(i=1 to a-1){n-i} + (b-a)
   * step 1: let a = 0, cpx = x
   * step 2: subtract (n-a-1) from cpx
   * step 3: add 1 to a
   * step 4: if cpx > 0 goback to step2
   * step 5: leb b = cpx + n
   * return pair (a,b)
   */
  pair<int,int> decodeSwapCode(int swapCode, int n);
}

namespace MDPHelper{
  double getReward(Tour& pi_star, ReinLearnMemory& RLmemory);

  /**
   * Swap node p and q in pi
   * and return added dist, dropped dist as pair
   * 
   * let swap is (p,q) and p < q in general
   * let i := pi_inv[p], j := pi_inv[q]
   * let d(l,k) means dist(g.getNode(pi[l]), g.getNode(pi[k])) 
   * 
   * First, calculate addedDist
   * it is d(i-1,j) + d(j,i+1) + d(j-1,i) + d(i,j+1)
   * Next, calculate droppedDist
   * it is d(i-1,i) + d(i,i+1) + d(j-1,j) + d(j,j+1)
   * 
   * Then, swap pi[i] and pi[j]
   * and pi_inv[p] := j, pi_inv[q] := i
   * 
   * Finally return pair (addedDist, droppedDist)
   */
  pair<double,double> subPerturb(vector<int>& pi, vector<int>& pi_inv, const pair<int,int>& swap, const Arguments& tspArgs);

  /**
   * There are 6 elements in action's feature vector
   * call each of them f_1, ... , f_6
   * and let action's feature vector f_a
   * 
   * information that are needed to calculate f_a is below
   *  1. the numbers of swap in swaps
   *  2. sum of added weights 
   *  3. sum of dropped weights
   *  4. lastTimeNodeActioned
   *  5. time
   * 1. is in Action a
   * 2. and 3. are calculated be subPerturbs
   * 4. and 5. are stored in RLmemory
   * 
   * ## detail about f_4, f_5, f_6 ##
   * let "swaps" is std::vector of std::pair<int,int>
   * let "P" is set of every integral in swaps
   *   ex) swaps = {{2,10}, {6,9}} -> P = {2,10,6,9}
   * let "t" is RLmemory.time
   * let "Tau" is RLmemory.lastTimeNodeActioned
   * let "Tau(P)" is set {Tau(p) | p \in P}
   * 
   * Then f_4 is min(t - Tau(P))
   *   It is equal to t - max(Tau(P))
   * f_5 is average(t - Tau(P))
   *   It is equal to t - average(Tau(P))
   * f_6 is max(t - Tau(P))
   *   It is equal to t - min(Tau(P))
   * 
   * So, while calculating average(Tau(P)),
   * max(Tau(P)), min(Tau(P)) can be also determined
   */
  vector<double> getActionFeatures(Action& a, State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs);
  vector<double> getActionFeatures(const vector<pair<int,int> >& swaps, Tour& pi_star, ReinLearnMemory& RLmemory, const Arguments& tspArgs);
}

class State{
  public: //instances
    Tour pi;
    Tour pi_star;
  
  public:
    // constructor
    State(Tour pi, Tour pi_star); 

    // getter 
    Tour getPi();
    Tour getPiStar();

    // perturber
    Tour perturb(Action& a, const Arguments& tspArgs);
    Tour perturb(vector < pair<int,int> >& swaps, const Arguments& tspArgs);
};

class Action{
  private:
    int sigma; // the number of swap
    vector<pair <int, int> > swaps; // pairs of swapped indexes

  public:
    Action(State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs);
    vector<pair <int, int> > getSwaps();
};

class MDP{
  private:
    unsigned int time;
    State state;
    Action action;
    double reward;
};

#endif // TSP_MDP_H