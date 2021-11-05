#include "./MarkovDecisionProcess.h"
#include "../model/Arguments.h"
#include "../model/Tour.h"
#include "../model/Graph.h"
#include "../model/Distance.h"

#include <vector>
#include <iostream> // std::cout
#include <utility> // std::pair
#include <limits.h> // INT_MAX

using namespace std;

//==== MDPHelper ================================
pair<double,double> MDPHelper::subPerturb(vector<int>& pi, vector<int>& pi_inv, const pair<int,int>& swap, const Arguments& tspArgs){
  int p_index = swap.first;
  int q_index = swap.second;

  //Exception Check : when p >= q
  if(p_index >= q_index){
    cout << "ERROR : MDPHelper::subPerturb input swap error" << endl;
    cout << "p is greater than q. please check input argument" << endl;
    exit(1);
  }

  int i_order = pi_inv[p_index];
  int j_order = pi_inv[q_index];

  // Calculate addedDist, dropped Dist
  Node i_minus_node = tspArgs.V.getNode(pi[i_order-1]);
  Node i_node = tspArgs.V.getNode(pi[i_order]);
  Node i_plus_node = tspArgs.V.getNode(pi[i_order+1]);
  Node j_minus_node = tspArgs.V.getNode(pi[j_order-1]);
  Node j_node = tspArgs.V.getNode(pi[j_order]);
  Node j_plus_node = tspArgs.V.getNode(pi[j_order+1]);

  double addedDist = 0;
  addedDist += dist(i_minus_node,j_node);
  addedDist += dist(j_node,i_plus_node);
  addedDist += dist(j_minus_node,i_node);
  addedDist += dist(i_node, j_plus_node);

  double droppedDist = 0;
  droppedDist += dist(i_minus_node,i_node);
  droppedDist += dist(i_node,i_plus_node);
  droppedDist += dist(j_minus_node,j_node);
  droppedDist += dist(j_node, j_plus_node);

  pair<double,double> rstPair = make_pair(addedDist, droppedDist);

  // Swap p_index and q_index in pi, pi_inv
  pi[i_order] = q_index;
  pi[j_order] = p_index;
  pi_inv[q_index] = i_order;
  pi_inv[p_index] = j_order;

  return rstPair;
}

vector<double> MDPHelper::getActionFeatures(Action& a, State& s, ReinLearnMemory& RLmemory,const Arguments& tspArgs){
  vector< pair<int,int> > swaps = a.getSwaps();
  Tour pi_star = s.getPiStar();
  return getActionFeatures(swaps,pi_star,RLmemory,tspArgs);
}
vector<double> MDPHelper::getActionFeatures(const vector<pair<int,int> >& swaps, Tour& pi_star, ReinLearnMemory& RLmemory,const Arguments& tspArgs){
  double f1,f2,f3,f4,f5,f6;

  // calculate f1
  int sigma = swaps.size();
  f1 = sigma;

  // calculate f2, f3
  pair<double,double> addDropWeightsInfo = make_pair(0.0,0.0);
  double addedWeights = 0.0;
  double droppedWeights = 0.0;
  vector<int> pi_vec;
  vector<int> pi_inv_vec;
  StateHelper::initPiAndPiInv(pi_star,pi_vec,pi_inv_vec);
  for(auto swap : swaps){
    addDropWeightsInfo = MDPHelper::subPerturb(pi_vec, pi_inv_vec, swap, tspArgs);
    addedWeights += addDropWeightsInfo.first;
    droppedWeights += addDropWeightsInfo.second;
  }

  f2 = addedWeights;
  f3 = droppedWeights;

  // calculate f4, f5, f6
  vector<int> P;
  P.reserve(sigma * 2);
  for(auto swap : swaps){
    P.emplace_back(swap.first);
    P.emplace_back(swap.second);
  }

  int t = RLmemory.time;
  vector<int> *Tau_pt = &(RLmemory.lastTimeNodeActioned);
  int minTau = INT_MAX;
  int maxTau = -1;
  int sumTau = 0;
  double averageTau;

  for(int p : P){
    int TauP = (*Tau_pt)[p];
    if(TauP < minTau) minTau = TauP;
    if(TauP > maxTau) maxTau = TauP;
    sumTau += TauP;
  }

  averageTau = (double)(sumTau) / P.size();

  f4= (double)(t-maxTau);
  f5= (double)t - averageTau;
  f6= (double)(t-minTau);

  // merge f1 ~ f6 into actionFeatures and return
  vector<double> actionFeatures = {f1,f2,f3,f4,f5,f6};
  return actionFeatures;
}

//==== State ====================================
State::State(Tour _pi, Tour _pi_star){
  this->pi = _pi;
  if(_pi_star.isLocalOpt()){
    this->pi_star = _pi_star;
  } else {
    cout << "ERROR : State's constructor input error" << endl;
    cout << "Please check _pi_star is locar optimal" << endl;
    exit(1);
  }
};

Tour State::getPi(){
  return this->pi;
};
Tour State::getPiStar(){
  return this->pi_star;
};

// pi_tour is not const!!!!
// BE CAREFUL TO HANDLE IT
void StateHelper::initPiAndPiInv(Tour& pi_tour, vector<int>& pi_vec, vector<int>& pi_inv_vec){
  int n= pi_tour.getSize();
  pi_vec.clear();
  pi_inv_vec.clear();
  
  pi_vec.reserve(n+2); // pi_vec[0] and pi_vec[n+1]
  pi_inv_vec.reserve(n+1); // pi_inv_vec[0]

  pi_vec.emplace_back(pi_tour.pi(n)); 
  for(int pvi=1;pvi<n+1;pvi++) pi_vec.emplace_back(pi_tour.pi(pvi));
  pi_vec.emplace_back(pi_tour.pi(1));

  pi_inv_vec.assign(n+1,0); // initialize to zeros
  for(int pivi=1;pivi<n+1;pivi++){
    pi_inv_vec[pi_vec[pivi]]=pivi;
  } 
}

void StateHelper::eraseDummiesInPiVec(vector<int>& pi_vec){
  pi_vec.pop_back();
  pi_vec.erase(pi_vec.begin());
}

/*
Tour State::perturb(Action& a, const Arguments& tspArgs){
  vector<pair<int,int> > swapsCopy = a.getSwaps();
  return State::perturb(swapsCopy,tspArgs);
}
Tour State::perturb(vector<pair<int,int> >& swaps, const Arguments& tspArgs){
  vector<int> pi_vec, pi_inv_vec;
  StateHelper::initPiAndPiInv(this->pi_star, pi_vec, pi_inv_vec);
  for(auto swap : swaps){
    StateHelper::subPerturb(pi_vec,pi_inv_vec,swap);
  }
  StateHelper::tidyPiVec(pi_vec);
  Tour perturbedTour(pi_vec,tspArgs.V);

  return perturbedTour;
}
*/

//==== Action ===================================
int ActionHelper::encodeSwap(pair<int,int> swap, int n){
  int a = swap.first;
  int b = swap.second;
  //Exception check
  if(a >= b){
    cout << "ERROR: ActionHelper::encodeSwap invalid swap" << endl;
    cout << "swap.first is greater than swap.second" << endl;
    exit(1);
  }

  return (a-1)*n - (a*a-a)/2 + (b-a);
}

pair<int,int> ActionHelper::decodeSwapCode(int swapCode, int n){
  int a=0;
  int cpx = swapCode;
  
  while(cpx > 0){
    cpx -= (n-a-1);
    a++;
  }

  int b=cpx+n;

  //Exception Check
  if(b <= a){
    cout << "ERROR: ActionHelper::decodeSwapCode result error" << endl;
    cout << "b is lesser than a in pair (a,b) " << endl;
    exit(1);
  }

  return pair<int,int>(a,b);
}

vector<pair <int,int> > Action::getSwaps(){
  return this->swaps;
}

//==== MDP ===+==================================
