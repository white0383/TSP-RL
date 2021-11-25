#include "LinearFittedQIteration.h"
#include "../helper/mt19937ar.h"
#include "../model/Arguments.h"
#include "../solver/initial_solution/GenerateInitialSolution.h"
#include "../helper/RandVec.h"
#include "../helper/fitLinearQ.h"

#include <ctime>
#include <vector>
#include <deque>
#include <cfloat> // DBL_MAX
#include <iostream>
#include <random> // mt19937, normal_distribution

using namespace std;

//========= LinQHelper ===============================================
namespace LinQHelper {

//===================== in LinQ Constructor =============================
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

  deque<MDP> genInitReplayBuffer(){
    deque<MDP> rstDeq;
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

  vector<int> genVsmp(const Arguments& tspArgs){
    vector<int> rstVec = genRandDiffIntVecBySet(tspArgs.V.getN(), tspArgs.KSMP);
    return rstVec;
  }

//===================== in LinQ.Learn ==================================


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
  this->V_smp = LinQHelper::genVsmp(tspArgs);
};

void LinearFittedQIteration::learn(const Arguments& tspArgs){
  cout << "learn is fun" << endl;
  Tour pi_init = generateInitialSolution(tspArgs);
  pi_init.setScaledCost(tspArgs.V);
  State s_prev = State(pi_init,tspArgs);
  this->distQueue.push_back(s_prev.distPiStar);
  this->bestTour = s_prev.pi_star;

  //bool testOK = true;
  //int failCount = 0;

  while(LinearFittedQIteration::checkTerminationCondition(tspArgs) == false){
    for(this->step = 1;this->step <= this->MAXstep;this->step++){
      cout << "#####time : " << this->time << " begin " << endl;
      Action a_prev = Action(s_prev, *this, tspArgs);
      State s_next = State(s_prev, a_prev, *this,tspArgs);

      /* perturb test
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
        testOK = false;
        failCount ++;
        cout << "perturb ERROR!!!" << endl;
        cout << "pi : mine : " << pi_scost << endl;
        cout << "     real : " << pi_real_scost << endl;
        cout << "ps : mine : " << pistar_scost << endl;
        cout << "     real : " << pistar_real_scost << endl;
        cout << "pert : mine : " << pt_gap << endl;
        cout << "       real : " << pt_real_gap << endl;
        exit(1);
      } else {
        //cout << "perturb OOKKK!!!" << endl;
      }
      */
      
      double r_prev = MDPHelper::getReward(s_next,*this,tspArgs);

      /* State Feature test
      vector<double> f_s = MDPHelper::getStateFeatures(s_prev,*this,tspArgs);
      cout << "STATE features" << endl;
      s_prev.pi_star.printTour();
      cout << "dimention : " << f_s.size() << endl;
      cout << "f1 : " << f_s.at(0) << " f2 : " << f_s.at(1) << " f3 : " << f_s.at(2) << endl;
      cout << "f3i : " ;
      for(int ii = 0; ii < tspArgs.KSMP ; ii++) cout << f_s.at(3+ii) << " ";
      cout << endl;
      cout << "f3KSMPj : ";
      for(int ii = 0; ii < tspArgs.OMEGA ; ii++) cout << f_s.at(3+tspArgs.KSMP+ii) << " ";
      cout << endl;
      */

      vector<double> f_prev = MDPHelper::getFeatureVector(s_prev, a_prev, *this, tspArgs);
      MDP mdp_prev = MDP(s_prev, s_next, a_prev, r_prev, f_prev, *this);
      this->updateInfo(mdp_prev, s_prev, s_next, tspArgs);
    }
    DataSet dataset = DataSet(tspArgs, *this);
    //this->updateWeights(dataset);
    this->epi++;
  }

  //if(testOK == true){
  //  cout << "test OK" << endl;
  //} else {
  //  cout << "test Fail" << endl;
  //  cout << "fail num : " << failCount << endl;
  //}

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

void LinearFittedQIteration::updateInfo(MDP& mdp, State& s_prev, State& s_next, const Arguments& tspArgs){
  // update replayBuffer
  this->updateReplayBuffer(mdp, tspArgs);

  // update lastTimeNodeActioned
  this->updateLastTimeNodeActioned(mdp.a_prev);

  // update bestTime, bestDist
  this->updateBestInfos(s_prev);

  // update distQueue
  //updateDistQueue(double dist_piStar_next, const Arguments& tstArgs){
  this->updateDistQueue(s_next.distPiStar, tspArgs);

  // s_next is next s_prev
  s_prev = s_next;

  // time inscrease
  this->time++;
}

void LinearFittedQIteration::updateReplayBuffer(MDP& prevMDP,const Arguments& tspArgs){
  this->replayBuffer.push_back(prevMDP);
  if(this->replayBuffer.size() > tspArgs.MMAX){
    this->replayBuffer.pop_front();
  }
}

void LinearFittedQIteration::updateLastTimeNodeActioned(Action& a_prev){
  for(pair<int,int> swap : a_prev.getSwaps()){
    this->lastTimeNodeActioned.at(swap.first) = this->time;
    this->lastTimeNodeActioned.at(swap.second) = this->time;
  }
}

void LinearFittedQIteration::updateBestInfos(State& s_prev){
  double dist_piStar_prev = s_prev.distPiStar;

  if(dist_piStar_prev < this->bestDist){
    this->bestDist = dist_piStar_prev;
    this->bestTime = this->time;
    this->bestTour = s_prev.pi_star;
  }
}

void LinearFittedQIteration::updateDistQueue(double dist_piStar_next, const Arguments& tspArgs){
  this->distQueue.push_back(dist_piStar_next);
  if(this->distQueue.size() > tspArgs.THETA){
    this->distQueue.pop_front();
  }
}

void LinearFittedQIteration::updateWeights(DataSet& dataSet){
  this->weights = fitLinearQ(dataSet.featureVectors, dataSet.targetValues);
}


//========= DataSet Member ===========================================
DataSet::DataSet(const Arguments& tspArgs, LinearFittedQIteration& LinQ){
  deque<MDP>& replayBuffer = LinQ.replayBuffer;

  this->sampleIndexs = genRandDiffIntVecBySet(0, replayBuffer.size()-1, tspArgs.T);
  this->targetValues.reserve(tspArgs.T);
  this->featureVectors.reserve(tspArgs.T);

  double h = 0.0;
  double targetValue = 0.0;

  for(int sampleIndex : sampleIndexs){
    Action a_sampled = Action(replayBuffer.at(sampleIndex).s_next, LinQ, tspArgs);
    h = MDPHelper::Qfunction(replayBuffer.at(sampleIndex).s_next, a_sampled, LinQ, tspArgs);

    targetValue = replayBuffer.at(sampleIndex).r_prev + tspArgs.GAMMA * h;
    vector<double> f_sampled = replayBuffer.at(sampleIndex).f_prev;

    this->targetValues.emplace_back(targetValue);
    this->featureVectors.emplace_back(f_sampled);
  }
}