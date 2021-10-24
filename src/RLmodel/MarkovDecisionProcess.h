#ifndef TSP_MDP_H
#define TSP_MDP_H

#include <vector>

class State{

};

class Action{

};

class MDP{
  private:
    unsigned int time;
    State state;
    Action action;
    double reward;
    vector<double> feature;
}

#endif // TSP_MDP_H