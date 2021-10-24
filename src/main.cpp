#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "./model/Graph.h"
#include "./model/Tour.h"
#include "./model/Arguments.h"
#include "./solver/initial_solution/GenerateInitialSolution.h"
#include "./solver/local_search/SearchLocalOpt.h"

using namespace std;

vector<double> learnLinearQfunction(const Arguments& tspArgs){
  vector<double> weight = generateInitialWeight(tspArgs);
  Tour pi_init = generateInitialSolution(tspArgs);
  unsigned int ite = 1;
  clock_t start = clock();

  while(isNotSatisfiedTerminationCondition(tspArgs,ite,start)){
    // Sar : struct of pair<Tour, Tour>, vector<double>, double 
    vector< Sar > replayBuffer = generateReplayBuffer(weight, pi_init, tspArgs);
    vector< Sar > samples = chooseSample(replayBuffer, tspArgs);
    vector<double> targetValues = generateTargetValues(samples, weight);
    weight = learnWeightByLSM(samples, weight, targetValues);
    pi_init << getNextPiInit(replayBuffer);// 演算子オーバロードでなんとかできるはず
  }

  return weight;
}

int main(int argc, char** argv){
  /************************************/
  /**** Set your test config here. ****/
  /************************************/
  vector<string> tmpSTR = {"xqg237.tsp", "FI", "F2OPT", "ITE"};
  vector<unsigned int> tmpINT = {1235, 100, 500, 10, 1000000};
  vector<double> tmpREA = {0.95, 0.95, 123.1};
  Arguments tmpArgs = Arguments(tmpSTR, tmpINT, tmpREA);

  //Reinforcement Learning
  vector<double> weight = learnLinearQfunction(tmpArgs);

  return 0;
}

/**
 * Memo 
 * (written 20210919 00:28 )
 * (modified 20210920 16:41 )
 * (完了) 1. Random Tour 関数
 * (完了) 3. Local Search 関数
 * (完了) 4. 2opt 関数
 * (完了) 5. Tour.printTour メソッド
 *    1->4->13-> ... ->7->1 このように出力し
 *    コストも出力
 * (完了)9. cの性能低いrandを <random>の mt19937 の全域関数版にする
 * ------ 今後の予定
 * 2. Nearest Neighbor 関数
 * 6. Usage 作成
 * 7. main関数 try-catchにする
 * 8. 各部分に throw 作成
 */
