#ifndef TSP_RLMEMORY_H
#define TSP_RLMEMORY_H

#include <vector>
#include <queue>
#include <ctime>
#include "../model/Arguments.h"
#include "../model/Tour.h"

class Episode; //predeclaration
class ReinLearnMemory{
  public:
    vector<double> weights;
    Tour pi_init;
    unsigned int epi;
    unsigned int MAXepi;
    time_t startSec;
    double spendSec;
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

    // Getter
    vector<double> getWeights();

    //For Debugging
    void setPiInitCost(const Arguments& tspArgs);
    Tour getPiInit();
    void printWeights();
};

#endif //TSP_RLMEMORY_H