#include <iostream>
#include <string>
#include <vector>

#include "./model/Graph.h"
#include "./model/Tour.h"
#include "./model/Arguments.h"
#include "./solver/initial_solution/GenerateInitialSolution.h"
#include "./solver/local_search/SearchLocalOpt.h"

using namespace std;

int main(int argc, char** argv){
  vector<string> tmpSTR = {"xqg237.tsp", "RT", "2OPT"};
  vector<unsigned int> tmpNAT = {1235, 100, 500, 10, 1000000};
  vector<double> tmpREA = {0.95, 0.95, 123.1};

  Arguments tmpArgs = Arguments(tmpSTR, tmpNAT, tmpREA);

  Tour pi_init3 = generateInitialSolution(tmpArgs);
  pi_init3.setCost(tmpArgs.V);
  pi_init3.printTour();
  
  //STEP3 : 局所探索
  Tour pi_localOpt3 = searchLocalOpt(tmpArgs, pi_init3);
  pi_localOpt3.setCost(tmpArgs.V);
  pi_localOpt3.printTour();

  //##########여기부터 강화학습파트#######
  //1. 무작위 weight로 초기화
  //vector<double> weight = generateInitialWeight();
  //2. 초기 상태 s_init생성


  //##########강화학습 끝! ############


  return 0;
}

/**
 * Memo 
 * (written 20210919 00:28 )
 * (modified 20210920 16:41 )
 * (完了) 1. Random Tour 関数
 * (完了) 3. Local Search 関数
 * (完了) 4. 2opt 함수
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