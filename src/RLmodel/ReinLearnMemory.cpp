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
#include <random> // std::normal_distribution
#include <deque>

using namespace std;

namespace reinLearnMemoryHelper{
  /**
   * Generate weight vecotr which has random double-type values
   * there values are included in [ (+-) tspArgs.WEIGHTINTERVAL/2]
   */
  vector<double> genRandomWeights_Uniform(const Arguments& tspArgs){
    vector<double> rst_weights;
    rst_weights.reserve(tspArgs.K + 1);
    double weight =0.0;
    for(int i=0;i<tspArgs.K +1;i++){
      weight = (genrand_real1()-0.5 ) * tspArgs.WEIGHTS_INITPARA;
      rst_weights.push_back(weight);
    }
    return rst_weights;
  }

  vector<double> genRandomWeights_Gaussian(const Arguments& tspArgs){
    vector<double> rst_weights;
    rst_weights.reserve(tspArgs.K + 1);
    // set gaussian RNG
    mt19937 gen(genrand_int32());
    normal_distribution<double> gauss(0,tspArgs.WEIGHTS_INITPARA);
    double weight =0.0;
    for(int i=0;i<tspArgs.K +1;i++){
      weight = gauss(gen);
      rst_weights.push_back(weight);
    }
    return rst_weights;    
  }

  vector<double> genRandomWeights(const Arguments& tspArgs){
    if(tspArgs.WEIGHT_INIT_METHOD == "UNIFM"){
      return genRandomWeights_Uniform(tspArgs);
    } else if (tspArgs.WEIGHT_INIT_METHOD == "GAUSS"){
      return genRandomWeights_Gaussian(tspArgs);
    }

    // statements below are just for c++ grammar
    // Only when Acguments' initialization has problem, 
    // below statement will run.
    vector<double> rst_dummy;
    cout << "ERROR : reinLearnMemoryHelper::genRandomWeights invalid Arguments' WEIGHT_INIT_METHOD" << endl;
    return rst_dummy;   
  };

  vector<int> genSizeNZeroVector(const Arguments& tspArgs){
    vector<int> rstVector(tspArgs.V.getN() + 1);
    return rstVector;
  };

  deque<double> genInitDistQueue(){
    deque<double> rst_Queue;
    rst_Queue.push_back(DBL_MAX); // d(pi_0_star) = DBL_MAX
    return rst_Queue;
  } ;
}


ReinLearnMemory::ReinLearnMemory(const Arguments& tspArgs){
  this->weights = reinLearnMemoryHelper::genRandomWeights(tspArgs);
  this->pi_init = generateInitialSolution(tspArgs);
  this->epi = 1;
  this->startSec_timeT = clock();
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

  // statements below are just for c++ grammar
  // Only when Acguments' initialization has problem, 
  // below statement will run.
  cout << "Error : ReinLearnMemory::checkTerminationCondition invalid termination condition " << endl;
  cout << "Your input TERMINATE_METHOD is \"" << tspArgs.TERMINATE_METHOD <<"\""<< endl;
  exit(1);
}

Tour ReinLearnMemory::getPiInit(){
  return this->pi_init;
}

vector<double> ReinLearnMemory::getWeights(){
  return this->weights;
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