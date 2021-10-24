#ifndef TSP_RLMEMORY_H
#define TSP_RLMEMORY_H

#include <vector>
#include <queue>
#include "../model/Arguments.h"
#include "../model/Tour.h"

class Episode; //predeclaration
class ReinLearnMemory{
  private:
    vector<double> weights;
    Tour pi_init;
    unsigned int epi;
    double spendSec;
    unsigned int MAXepi;
    double MAXsec;
    unsigned int time;

    //For calculating feature vector and reward
    unsigned int bestTour_time;
    double bestTour_dist;
    vector<int> lastTimeNodeActioned;
    queue<double> distQueue;

  public:
    ReinLearnMemory(const Arguments& tspArgs);

    /**
     * check termination condition is satisfied or not
     * return true when algorithm should be terminated
     */
    bool checkTerminationCondition(const Arguments& tspArgs);

    /**
     * set new vector "weights" by LSM
     */
    void learnWeightByLSM(const Episode& episode, const Arguments& tspArgs);

    /**
     * set "pi_init" to Episode.replayBuffer.back().state.pi
     */
    void setNextPiInit(const Episode& episode);

    //For Debugging
    void setPiInitCost(const Arguments& tspArgs);
    Tour getPiInit();
    void printWeights();
};

#endif //TSP_RLMEMORY_H