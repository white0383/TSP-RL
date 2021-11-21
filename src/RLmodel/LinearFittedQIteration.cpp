#include "LinearFittedQIteration.h"
#include "../helper/mt19937ar.h"
#include "../model/Arguments.h"

#include <ctime>
#include <vector>
#include <deque>
#include <cfloat> // DBL_MAX
#include <iostream>
#include <random> // mt19937, normal_distribution

using namespace std;

//========= class LinQ ==========
namespace LinQHelper {
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

  /**
   * Generate weight vecotr which has gaussian double-type values
   * each of them are i.i.d of N(0,WEIGHTS_INITPARA)
   */
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

  vector<MDP> genInitReplayBuffer(){
    vector<MDP> rstDeq;
    return rstDeq;
  }

  vector<unsigned int> genInitLastTimeNodeActioned(const Arguments& tspArgs){
    vector<unsigned int> rstVec(tspArgs.V.getN() + 1);
    return rstVec;
  }

  deque<double> genInitDistQueue(const Arguments& tspArgs){
    deque<double> rstDeq;
    // rstDeq = {+infinity} = {dist(pi_star_0)}
    rstDeq.push_back(DBL_MAX); 
    return rstDeq;
  }
}

LinearFittedQIteration::LinearFittedQIteration(const Arguments& tspArgs){
  this->weights = LinQHelper::genRandomWeights(tspArgs);
  this->replayBuffer = LinQHelper::genInitReplayBuffer();

  this->startTimeT = clock();
  this->spendSec = 0.0;
  this->MAXspendSec = tspArgs.SEC_LIMIT;

  this->time = 1;
  this->step = 1;
  this->MAXstep = tspArgs.TMAX;
  this->epi = 1;
  this->MAXepi = tspArgs.EPI_LIMIT;

  this->bestTime = 0;
  this->bestDist = DBL_MAX;

  this->lastTimeNodeActioned = LinQHelper::genInitLastTimeNodeActioned(tspArgs);
  this->distQueue = LinQHelper::genInitDistQueue(tspArgs);
};

//======== class DataSet ========