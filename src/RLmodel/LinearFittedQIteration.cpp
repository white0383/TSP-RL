#include "LinearFittedQIteration.h"
#include "../helper/mt19937ar.h"
#include "../model/Arguments.h"
#include "../solver/initial_solution/GenerateInitialSolution.h"

#include <ctime>
#include <vector>
#include <deque>
#include <cfloat> // DBL_MAX
#include <iostream>
#include <random> // mt19937, normal_distribution

using namespace std;

//========= LinQHelper ===============================================
namespace LinQHelper {

//===================== LinQ Constructor =============================
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

//===================== LinQ.Learn ==================================


}

//========= LinQ Member ==============================================
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

void LinearFittedQIteration::learn(const Arguments& tspArgs){
  cout << "learn is fun" << endl;
  Tour pi_init = generateInitialSolution(tspArgs);
  pi_init.setScaledCost(tspArgs.V);
  State s_prev = State(pi_init,this->time,tspArgs);
  this->distQueue.push_back(s_prev.distPiStar);

  while(LinearFittedQIteration::checkTerminationCondition(tspArgs) == false){
    for(this->step = 1;this->step <= this->MAXstep;this->step++){
      cout << "#####time : " << this->time << " begin " << endl;
      Action a_prev = Action(s_prev, *this, tspArgs);
      State s_next = State(s_prev, a_prev, this->time, tspArgs);

      Tour pi_cp = s_prev.getPi();
      Tour pistar_cp = s_prev.getPiStar();
      pi_cp.setScaledCost(tspArgs.V);
      pistar_cp.setScaledCost(tspArgs.V);
      double pi_scost = s_prev.pi.getScaledCost();
      double pistar_scost = s_prev.pi_star.getScaledCost();
      double pi_real_scost = pi_cp.getScaledCost();
      double pistar_real_scost = pistar_cp.getScaledCost();

      //double to_gap = pi_scost - pistar_scost;
      //double to_real_gap = pi_real_scost - pistar_real_scost;
      //cout << "2opt : mine : " << to_gap << endl;
      //cout << "       real : " << to_real_gap << endl;

      Tour pinext_cp = s_next.getPi();
      pinext_cp.setScaledCost(tspArgs.V);

      double pinext_scost = s_next.pi.getScaledCost();
      double pinext_real_scost = pinext_cp.getScaledCost();
      double pt_gap = pinext_scost - pistar_scost;
      double pt_real_gap = pinext_real_scost - pistar_real_scost;
      if(abs(pt_gap - pt_real_gap) >= 0.001) {
        cout << "perturb ERROR!!!" << endl;
        cout << "pi : mine : " << pi_scost << endl;
        cout << "     real : " << pi_real_scost << endl;
        cout << "ps : mine : " << pistar_scost << endl;
        cout << "     real : " << pistar_real_scost << endl;
        cout << "pert : mine : " << pt_gap << endl;
        cout << "       real : " << pt_real_gap << endl;
      } else {
        cout << "perturb OOKKK!!!" << endl;
      }
      

      s_prev = s_next;
      double r_prev = MDPHelper::getReward(s_next,*this,tspArgs);

      //vector<double> f_prev = MDPHelper::getFeatureVector(s_prev, a_prev, *this);
      //MDP mdp_prev = MDP(s_prev, a_prev, r_prev, s_next, f_prev, *this);
      //this->updateModelInfo(mdp_prev, tspArgs, s_prev, s_next)
      
      //FOR DEBUG
      cout << "#####time : " << this->time << " finish " << endl << endl;
      this->time++;
      this->distQueue.push_back(s_prev.distPiStar);
    }
    //DataSet dataset = DataSet(this->weights, this->replayBuffer, tspArgs)
    //this->updateWeights(dataset);
    this->epi++;
  }

}

  bool LinearFittedQIteration::checkTerminationCondition(const Arguments& tspArgs){
    if(tspArgs.TERMINATE_METHOD == "EPI"){
      return (this->epi > this->MAXepi);
    }else if(tspArgs.TERMINATE_METHOD == "SEC"){
      return (this->spendSec > this->MAXspendSec);
    }

    // statements below are just for c++ grammar
    // Only when Acguments' initialization has problem, 
    // below statement will run.
    cout << "Error : ReinLearnMemory::checkTerminationCondition invalid termination condition " << endl;
    cout << "Your input TERMINATE_METHOD is \"" << tspArgs.TERMINATE_METHOD <<"\""<< endl;
    exit(1);    
  }
//========= DataSet Member ===========================================
