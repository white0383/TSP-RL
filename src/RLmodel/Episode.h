#ifndef TSP_EPISODE_H
#define TSP_EPISODE_H

#include <vector>
#include <tuple>
#include <utility> // std::pair
#include "../model/Arguments.h"
#include "../model/Tour.h"
#include "ReinLearnMemory.h"
#include "MarkovDecisionProcess.h"

using namespace std;

class Episode{
  public:
    vector<MDP> replayBuffer;
    vector<int> samples; // just save index of replayBuffer for memory reduction
    vector<double> targetValues;
    vector< vector<double> > featureVectors;
    unsigned int step; // このエピソードで何番目のMDPか

  public:
    Episode(const Arguments& tspArgs);

    void generateReplayBuffer(ReinLearnMemory& RLmemory ,const Arguments& tspArgs);

    /**
     * set T dimention vector "samples"
     * it has T different int in [0, TMAX-1]
     * because "step" of each episode is in [0, TMAX-1]
     * 
     * choose first T member in shuffled [0, TMAX-1]
     */
    void selectSamples(const Arguments& tspArgs);

    /**
     * set T dimetion vector "targenValues"
     * it is \rho in Algorithm2
     */
    void generateTargetValues(ReinLearnMemory& RLmemory ,const Arguments& tspArgs);
};

#endif //TSP_EPISODE_H