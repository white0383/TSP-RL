#include "Episode.h"
#include "ReinLearnMemory.h"
#include "MarkovDecisionProcess.h"
#include "../helper/mt19937ar.h"
#include "../model/Arguments.h"
#include "../model/Graph.h"
#include "../model/Tour.h"
#include "../solver/local_search/SearchLocalOpt.h"

#include <vector>
#include <numeric> // std::iota, std::inner_product
#include <random>
#include <deque>

using namespace std;

namespace episodeHelper{
  deque<MDP> getInitReplayBuffer(const Arguments& tspArgs){
    deque<MDP> rstDeq;
    vector<MDP> rstVec;
    rstVec.reserve(tspArgs.TMAX);
    return rstDeq;
  };

  //initialized in Zeros
  vector<int> getInitSamples(const Arguments& tspArgs){
    vector<int> rstVec;
    rstVec.reserve(tspArgs.T);
    return rstVec;
  };

  vector<double> getInitTargetValues(const Arguments& tspArgs){
    vector<double> rstVec;
    rstVec.reserve(tspArgs.T);
    return rstVec;
  };

  vector< vector<double> > getInitFeatureVectors(const Arguments& tspArgs){
    vector< vector<double> > rstVec2;
    rstVec2.reserve(tspArgs.T);
    return rstVec2;
  }

  void updateReplayBuffer(deque<MDP>& repBuff, MDP newMDP, const Arguments& tspArgs){
    if(repBuff.size() == tspArgs.MMAX) repBuff.pop_front();
    repBuff.emplace_back(newMDP);
  }
};

Episode::Episode(const Arguments& tspArgs){
  this->replayBuffer = episodeHelper::getInitReplayBuffer(tspArgs);
  this->samples = episodeHelper::getInitSamples(tspArgs);
  this->targetValues = episodeHelper::getInitTargetValues(tspArgs);
  this->featureVectors = episodeHelper::getInitFeatureVectors(tspArgs);
  this->step = 1;
};

void Episode::selectSamples(const Arguments& tspArgs){
  // generate {0,1, ... , TMAX-1} and shuffle it
  vector<int> tmpVec(tspArgs.TMAX);
  iota(tmpVec.begin(), tmpVec.end(), 0);
  mt19937 mtRand(genrand_int32());
  shuffle(tmpVec.begin(), tmpVec.end(),mtRand);

  // copy first T members of tmpVec to this->samples
  for(int i=0;i<tspArgs.T;i++){
    this->samples.emplace_back(tmpVec[i]);
  }
}

/*
void Episode::generateTargetValues(ReinLearnMemory& RLmemory ,const Arguments& tspArgs){
  for(int i=0;i<tspArgs.T;i++){
    int sample_index = this->samples.at(i);
    State sample_state = this->replayBuffer.at(sample_index).state;
    Action sample_action = Action(sample_state, RLmemory, tspArgs);
    vector<double> sample_feature = featureMapping(sample_state, sample_action, RLmemory, tspArgs);
    this->featureVectors.emplace_back(sample_feature);
    double h = inner_product(sample_feature.begin(), sample_feature.end(), RLmemory.weights.begin(), 0);
    double rho_i = this->replayBuffer.at(sample_index).reward + h * tspArgs.GAMMA;
    this->targetValues.emplace_back(rho_i);
  }
}
*/