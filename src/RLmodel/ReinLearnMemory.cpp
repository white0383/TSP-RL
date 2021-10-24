#include "ReinLearnMemory.h"
#include "Episode.h" //predeclaration
#include "../model/Arguments.h"
#include "../model/Graph.h"
#include "../model/Tour.h"
#include "../solver/initial_solution/GenerateInitialSolution.h"
#include "../helper/mt19937ar.h"

#include <iostream>
#include <vector>
#include <float.h>
using namespace std;

namespace reinLearnMemoryHelper{
  /**
   * Generate weight vecotr which has random double-type values
   * there values are included in [ (+-) tspArgs.WEIGHTINTERVAL/2]
   */
  vector<double> genRandomWeights(const Arguments& tspArgs){
    vector<double> rst_weights;
    rst_weights.reserve(tspArgs.K + 1);
    double weight =0.0;
    for(int i=0;i<tspArgs.K +1;i++){
      weight = (genrand_real1()-0.5 ) * tspArgs.WEIGHT_INTERVAL;
      rst_weights.push_back(weight);
    }
    return rst_weights;
  };

  vector<int> genSizeNZeroVector(const Arguments& tspArgs){
    vector<int> rstVector(tspArgs.V.getN() + 1);
    return rstVector;
  };

  queue<double> genInitDistQueue(){
    queue<double> rstQueue;
    rstQueue.push(DBL_MAX); // d(pi_0_star = DBL_MAX)
    return rstQueue;
  } ;
}


ReinLearnMemory::ReinLearnMemory(const Arguments& tspArgs){
  this->weights = reinLearnMemoryHelper::genRandomWeights(tspArgs)
  this->pi_init = generateInitialSolution(tspArgs);
  this->epi = 1;
  this->spendSec = 0.0;
  this->MAXepi = tspArgs.EPI_LIMIT;
  this->MAXsec = tspArgs.SEC_LIMIT;
  this->time = 1;

  this->bestTour_time = 0;
  this->bestTour_dist = DBL_MAX;
  this->lastTimeNodeActioned = reinLearnMemoryHelper::genSizeNZeroVector(tspArgs);
  this->distQueue = reinLearnMemoryHelper::genInitDistQueue();
}

bool ReinLearnMemory::checkTerminationCondition(const Arguments& tspArgs){
  if(tspArgs.TERMINATE_METHOD == "EPI"){
    return (this->epi > this->MAXepi);
  }else if(tspArgs.TERMINATE_METHOD == "SEC"){
    return (this->spendSec > this->MAXsec);
  }
  //else -> input error
  cout << "Error : check input TERMINATE_METHOD is \"EPI\" or \"SEC\" " << endl;
  cout << "Your input TERMINATE_METHOD is \"" << tspArgs.TERMINATE_METHOD <<"\""<< endl;
  exit(1);
}

Tour ReinLearnMemory::getPiInit(){
  return this->pi_init;
}

// For Debugging
void ReinLearnMemory::setPiInitCost(const Arguments& tspArgs){
  this->pi_init.setCost(tspArgs.V);
}

void ReinLearnMemory::printWeights(){
  cout << "size -> " << this->weights.size() << endl;
  cout << "weights -> " << endl;
  for(auto w : this->weights) cout << w << " ";
  cout << endl;
}