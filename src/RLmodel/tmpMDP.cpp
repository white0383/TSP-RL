#include "tmpMDP.h"
#include "../model/Arguments.h"
#include "../model/Tour.h"
#include "../model/Graph.h"
#include "../model/Distance.h"
#include "../helper/RandVec.hpp"
#include "../helper/mt19937ar.h" // DEBUG

#include <vector>
#include <iostream> // std::cout
#include <utility> // std::pair
#include <set>
#include <algorithm> // std::min, std::max
#include <limits.h> // INT_MAX, INT_MIN
#include <cmath> // floor, sqrt
#include <float.h> // DBL_MAX, DBL_MIN

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

pair<double,double> MDPHelper::getAddedDroppedWeights(vector<int>& pi, vector<int>& pi_inv, const pair<int,int>& swap, const Arguments& tspArgs){
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
  //NO SWAP !!!!
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

double MDPHelper::evaluateActionFeatures(vector<double> actionFeatures, ReinLearnMemory& RLmemory,const Arguments& tspArgs){
  double rst_actionFeature_value = 0.0;

  auto ite_actionWeights_begin = 
    RLmemory.weights.begin() +
    1 +
    3 +
    tspArgs.KSMP +
    tspArgs.OMEGA;
  
  int afIndex = 0;
  for(auto ite_wa = ite_actionWeights_begin; ite_wa != RLmemory.weights.end(); ++ite_wa) rst_actionFeature_value += actionFeatures[afIndex++] * (*ite_wa); 

  return rst_actionFeature_value;
}

double MDPHelper::evaluateStateFeatures(vector<double> stateFeatures, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  double rst_stateFeature_value = 0.0;

  auto ite_stateWeights_begin =
    RLmemory.weights.begin() +
    1;
  auto ite_stateWeights_end = 
    RLmemory.weights.begin() +
    1 +
    3 +
    tspArgs.KSMP +
    tspArgs.OMEGA;
  
  int sfIndex = 0;
  for(auto ite_ws = ite_stateWeights_begin; ite_ws != ite_stateWeights_end; ++ite_ws) rst_stateFeature_value += stateFeatures[sfIndex++] * (*ite_ws); 

  return rst_stateFeature_value; 
}

double MDPHelper::Qfunction(Action& a, State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  double rst_qvalue=0.0;

  //get w_0
  double w_0 = RLmemory.weights[0];

  //calculate q_s
  //vector<double> stateFeatures = MDPHelper::getStateFeatures(??);
  //double qvalue_s = MDPHelper::evaluateStateFeatures(stateFeatures, RLmemory, tspArgs);

  //calculate q_a
  vector<double> actionFeatures = MDPHelper::getActionFeatures(a,s,RLmemory,tspArgs);
  double qvalue_a = MDPHelper::evaluateActionFeatures(actionFeatures,RLmemory,tspArgs);

  //sum them and return
  rst_qvalue = w_0 + qvalue_a; //+qvalue_s
  return rst_qvalue;
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

/* State::perturb
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
  int a,b;

  // a = ceil(0.5 * {(2n-1) - sqrt((2n-1)^2 - 8x) })
  a = (int)ceil(0.5 * ((2*n-1) - sqrt( (2*n-1) *(2*n-1) -8*swapCode)));
  b = swapCode - (n*(a-1) - (a*a + a)/2);

  //Exception Check
  if(b <= a){
    cout << "ERROR: ActionHelper::decodeSwapCode result error" << endl;
    cout << "b is lesser than a in pair (a,b) " << endl;
    exit(1);
  }

  return pair<int,int>(a,b);
}

vector<double> ActionHelper::getTmpActionFeatures(const vector<double>& actFeatures, vector<int>& pi_vec, vector<int>& pi_inv_vec, pair<int,int>& swap, ReinLearnMemory& RLmemory,const Arguments& tspArgs){
  double f1, f2,f3,f4,f5,f6;
  
  // calculate f1
  f1 = actFeatures[0] + 1;

  // calculate f2, f3
  pair<double,double> addDropWeightsInfo = MDPHelper::getAddedDroppedWeights(pi_vec, pi_inv_vec, swap, tspArgs);
  f2 = actFeatures[1] + addDropWeightsInfo.first;
  f3 = actFeatures[2] + addDropWeightsInfo.second;

  // calculate f4, f5, f6
  int p = swap.first;
  int q = swap.second;
  int t = RLmemory.time;
  int Tau_p = RLmemory.lastTimeNodeActioned.at(p);
  int Tau_q = RLmemory.lastTimeNodeActioned.at(q);
  double t_p = (double)(t - Tau_p);
  double t_q = (double)(t - Tau_q);
  f4 = min({actFeatures[3], t_p, t_q});
  double tmpSum = actFeatures[4] * (2 * actFeatures[0]) + t_p + t_q;
  f5 = tmpSum / (2*f1);
  f6 = max({actFeatures[5], t_p, t_q});

  // merge f1 ~ f6 into tmpActionFeatures and return
  vector<double> tmpActionFeatures = {f1,f2,f3,f4,f5,f6};
  return tmpActionFeatures;
}

void ActionHelper::updateActionFeatures(vector<double>& actFeatures, vector<int>& pi_vec, vector<int>& pi_inv_vec, pair<int,int>& swap, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  vector<double> updatedActFs = ActionHelper::getTmpActionFeatures(actFeatures, pi_vec,pi_inv_vec,swap,RLmemory,tspArgs);

  //update actFeature
  actFeatures.clear();
  actFeatures.assign(updatedActFs.begin(),updatedActFs.end());

  //update pi_vec and pi_inv_vec
  int p_index = swap.first;
  int q_index = swap.second;
  int i_order = pi_inv_vec[p_index];
  int j_order = pi_inv_vec[q_index];

  pi_vec[i_order] = q_index;
  pi_vec[j_order] = p_index;
  pi_inv_vec[q_index] = i_order;
  pi_inv_vec[p_index] = j_order;
}

vector<pair<int,int> > ActionHelper::genFullGreedy(Tour& pi_star, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  /* 
    swap is a pair of node's index which are going to be swapped
    code is encoded swap, so it is integer
  */

  //declare variables
  int n = tspArgs.V.getN();
  const int codeMax = (n*n -n) / 2; // Maximum swapCode
  int sigma = 0;
  const int sigmaMax = (int)(ceil(n/2.0));
  int bestCode = 0; // code which achived best score in while loop
  double tmp_score = 0;
  double bestScore = -(DBL_MAX); // the largest <f_act|w_act> until now
  double bestScore_prevSigma = 0;
  vector<double> actionFeatures = {0.0,0.0,0.0,DBL_MAX,0.0,-(DBL_MAX)}; // save best scored one
  vector<double> tmp_actionFeatures = {0.0,0.0,0.0,DBL_MAX,0.0,-(DBL_MAX)}; // used in for-loop
  vector<pair<int,int> > rst_swaps; // empty now
  set<int> codeSet; // empty now
  pair<int,int> tmp_swap; 

  //initialize pi and pi_inv
  vector<int> pi_vec;
  vector<int> pi_inv_vec;
  StateHelper::initPiAndPiInv(pi_star,pi_vec,pi_inv_vec);

  //search best swaps with FullGreedy rule
  while(sigma<sigmaMax){
    sigma ++;
    bestScore_prevSigma = bestScore;
    for(int tmp_code=1;tmp_code < codeMax+1;tmp_code++){
      //if code is in codeSet, continue
      if(codeSet.find(tmp_code) != codeSet.end()) continue;

      tmp_swap = ActionHelper::decodeSwapCode(tmp_code,n);
      tmp_actionFeatures = ActionHelper::getTmpActionFeatures(actionFeatures, pi_vec, pi_inv_vec, tmp_swap, RLmemory, tspArgs);
      tmp_score = MDPHelper::evaluateActionFeatures(tmp_actionFeatures,RLmemory,tspArgs);

      if(tmp_score > bestScore){
        bestScore = tmp_score;
        bestCode = tmp_code;
      }
    }

    //if there is no improvement on score,
    //stop further search
    if(bestScore == bestScore_prevSigma) break;

    pair<int,int> nextSwap = ActionHelper::decodeSwapCode(bestCode,n);
    codeSet.insert(bestCode);
    rst_swaps.push_back(nextSwap);
    ActionHelper::updateActionFeatures(actionFeatures, pi_vec, pi_inv_vec, nextSwap, RLmemory, tspArgs);
  }

  return rst_swaps;
}


vector<pair<int,int> > ActionHelper::genPartGreedy(Tour& pi_star, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  //declare variables
  int n = tspArgs.V.getN();
  const int codeMax = (n*n -n) / 2; // Maximum swapCode
  int sigma = 0;
  const int sigmaMax = (int)(ceil(n/2.0));
  int bestCode = 0; // code which achived best score in while loop
  double tmp_score = 0;
  double bestScore = -(DBL_MAX); // the largest <f_act|w_act> until now
  double bestScore_prevSigma = 0;
  vector<double> actionFeatures = {0.0,0.0,0.0,DBL_MAX,0.0,-(DBL_MAX)}; // save best scored one
  vector<double> tmp_actionFeatures = {0.0,0.0,0.0,DBL_MAX,0.0,-(DBL_MAX)}; // used in for-loop
  vector<pair<int,int> > rst_swaps; // empty now
  set<int> codeSet; // empty now
  pair<int,int> tmp_swap; 

  //initialize pi and pi_inv
  vector<int> pi_vec;
  vector<int> pi_inv_vec;
  StateHelper::initPiAndPiInv(pi_star,pi_vec,pi_inv_vec);

  while(sigma<sigmaMax){
    sigma ++;
    bestScore_prevSigma = bestScore;
    vector<int> tmp_codes = genRandDiffIntVecWithExceptionSec(codeMax, tspArgs.ASMP, codeSet);

    for(int tmp_code : tmp_codes){
      tmp_swap = ActionHelper::decodeSwapCode(tmp_code,n);
      tmp_actionFeatures = ActionHelper::getTmpActionFeatures(actionFeatures, pi_vec,pi_inv_vec,tmp_swap,RLmemory,tspArgs);
      tmp_score = MDPHelper::evaluateActionFeatures(tmp_actionFeatures, RLmemory, tspArgs);

      if(tmp_score > bestScore){
        bestScore = tmp_score;
        bestCode = tmp_code;
      }
    }

    //if there is no improvement on score, break
    if(bestScore == bestScore_prevSigma) break;

    pair<int,int> nextSwap = ActionHelper::decodeSwapCode(bestCode, n);
    codeSet.insert(bestCode);
    rst_swaps.push_back(nextSwap);
    ActionHelper::updateActionFeatures(actionFeatures, pi_vec, pi_inv_vec, nextSwap, RLmemory, tspArgs);
  }

  return rst_swaps;
}

vector<pair<int,int> > ActionHelper::genSampleGreedy(Tour& pi_star, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  //declare variables
  int n = tspArgs.V.getN();
  const int codeMax = (n*n -n) / 2; // Maximum swapCode
  int sigma = 1;
  const int sigmaMax = (int)(ceil(n/2.0));
  double tmp_score = 0;
  double bestScore = -(DBL_MAX); // the largest <f_act|w_act> until now
  vector<pair<int,int> > rst_swaps; // empty now
  vector<pair<int,int> > tmp_swaps;

  for(sigma=1;sigma < sigmaMax+1 ; sigma++){
    for(int i=0;i<tspArgs.ASMP;i++){
      vector<int> tmp_codes = genRandDiffIntVecBySet(codeMax, sigma);
      
      //decode tmp_codes -> tmp_swaps
      tmp_swaps.clear();
      for(int tmp_code : tmp_codes){
        tmp_swaps.emplace_back(ActionHelper::decodeSwapCode(tmp_code,n));
      }

      vector<double> tmp_actFs = MDPHelper::getActionFeatures(tmp_swaps, pi_star, RLmemory, tspArgs);
      tmp_score = MDPHelper::evaluateActionFeatures(tmp_actFs, RLmemory, tspArgs);

      if(tmp_score > bestScore){
        bestScore = tmp_score;
        rst_swaps.clear();
        rst_swaps.assign(tmp_swaps.begin(),tmp_swaps.end());
      }
    }
  }

  return rst_swaps;
}

vector<pair<int,int> > ActionHelper::genGreedy(State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  Tour pi_star = s.getPiStar();

  if(tspArgs.ACTION_GREEDY_METHOD == "FULL"){
    return ActionHelper::genFullGreedy(pi_star,RLmemory,tspArgs);
  } else if(tspArgs.ACTION_GREEDY_METHOD == "PART"){
    return ActionHelper::genPartGreedy(pi_star,RLmemory,tspArgs);
  } else if(tspArgs.ACTION_GREEDY_METHOD == "SAMP"){
    return ActionHelper::genSampleGreedy(pi_star,RLmemory,tspArgs);
  }

  // statements below are just for c++ grammar
  // Only when Acguments' initialization has problem, 
  // below statement will run.
  vector<pair<int,int> > rst_dummy;
  rst_dummy.push_back(make_pair(1,2));
  cout << "ERROR : ActionHelper::genGreedy invalid greedy method" << endl;
  return rst_dummy;
}

vector<pair<int,int> > ActionHelper::genRandom(const Arguments& tspArgs){
  int n=tspArgs.V.getN();
  const int codeMax = (n*n-n)/2;

  const int sigmaMax = (int)(ceil(n/2.0));
  const int sigma = (genrand_int31() % sigmaMax) + 1;

  vector<int> rand_codes = genRandDiffIntVecBySet(codeMax,sigma);
  vector<pair<int,int> > rst_swaps;
  rst_swaps.reserve(sigma);
  for(auto rand_code : rand_codes){
    rst_swaps.emplace_back(ActionHelper::decodeSwapCode(rand_code,n));
  }

  return rst_swaps;
}

vector<pair<int,int> > ActionHelper::genEpsGreedy(State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  double pr = genrand_real3();
  if(pr < tspArgs.EPS){
    return ActionHelper::genRandom(tspArgs);
  } else {
    return ActionHelper::genGreedy(s,RLmemory,tspArgs);
  }

  // statements below are just for c++ grammar
  // Only when Acguments' initialization has problem, 
  // below statement will run.
  vector<pair<int,int> > rst_dummy;
  rst_dummy.push_back(make_pair(1,2));
  cout << "ERROR : ActionHelper::genEpsGreedy invalid EPS value" << endl;
  return rst_dummy;  
}

Action::Action(State& s, ReinLearnMemory& RLmemory, const Arguments& tspArgs){
  this->swaps = ActionHelper::genEpsGreedy(s,RLmemory,tspArgs);
  this->sigma = this->swaps.size();
}

vector<pair <int,int> > Action::getSwaps(){
  return this->swaps;
}

int Action::getSigma(){
  return this->sigma;
}

//==== MDP ===+==================================