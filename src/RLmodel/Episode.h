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
  private:
    vector<MDP> replayBuffer;
    vector<int> samples; // just save index of replayBuffer for memory reduction
    vector<double> targetValues;
    unsigned int step; // このエピソードで何番目のMDPか

  public:
    Episode(const Arguments& tspArgs);

    void generateReplayBuffer(ReinLearnMemory& RLmemory ,const Arguments& tspArgs);

    /**
     * set T dimention vector "samples"
     * it has T different int in [1, TMAX]
     * 
     * choose first T member in shuffled [1, TMAX]
     */
    void selectSamples(ReinLearnMemory& RLmemory ,const Arguments& tspArgs);

    /**
     * set T dimetion vector "targenValues"
     * it is \rho in Algorithm2
     */
    void generateTargetValues(ReinLearnMemory& RLmemory ,const Arguments& tspArgs);
};

#endif //TSP_EPISODE_H