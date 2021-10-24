#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "./model/Arguments.h"
#include "./RLmodel/ReinLearnMemory.h"
#include "./RLmodel/Episode.h"

using namespace std;

/**
 * fitted Q iteration algorithm to learn a linear action value function
 */
vector<double> learnLinearQfunction(const Arguments& tspArgs){
  ReinLearnMemory RLmemory = ReinLearnMemory(tspArgs); // declare ok, imple ok, test ok

  while(RLmemory.checkTerminationCondition(tspArgs) == false){
    Episode episode = Episode(RLmemory, tspArgs); // declare ok, imple ok,
    episode.generateReplayBuffer(RLmemory, tspArgs); // declare ok
    episode.selectSamples(RLmemory, tspArgs); // declare ok, imple ok
    episode.generateTargetValues(RLmemory, tspArgs); // declare ok
    RLmemory.learnWeightByLSM(episode, tspArgs); 
    RLmemory.setNextPiInit(episode); 
    RLmemory.epi++;
  }

  return RLmemory.weights;
}


int main(int argc, char** argv){
  /************************************/
  /**** Set your test config here. ****/
  /************************************/
  vector<string> tmpSTR = {"xqg237.tsp", "FI", "F2OPT", "ITE"};
  vector<unsigned int> tmpINT = {1235, 100, 500, 10, 1000000};
  vector<double> tmpREA = {0.95, 0.95, 123.1, 100};
  Arguments tmpArgs = Arguments(tmpSTR, tmpINT, tmpREA);

  //Reinforcement Learning
  vector<double> weight = learnLinearQfunction(tmpArgs);

  return 0;
}
