#include <iostream>
#include <string>
#include <vector>

#include "./model/Graph.h"
#include "./helper/FileIOHelper.h"
#include "./helper/mt19937ar.h"
#include "./solver/initial_solution/GenerateInitialSolution.h"
#include "./solver/local_search/SearchLocalOpt.h"
#include "./model/Tour.h"

using namespace std;

int main(int argc, char** argv){
  /************************************/
  /**** Set your test config here. ****/
  /************************************/
  const string TSP_DATA_FILE = argv[1];
  const string INIT_SOLUTION_METHOD = argv[2];
  const string LOCAL_SEARCH_METHOD = argv[3];
  const unsigned int SEED = atoi(argv[4]);
  // const double TIME_LIMIT = atof(argv[5]);
  // const long long TOTAL_ITERATION = LL_MAX;

  // Set seed of random number generator
  init_genrand(SEED);

  //STEP1 : データの読み込み
  vector< vector <int> > nodes = readTSPFile(TSP_DATA_FILE);
  Graph cities = Graph(nodes);

  //STEP2 : 初期解生成
  Tour pi_init = generateInitialSolution(cities, INIT_SOLUTION_METHOD);
  pi_init.setCost(cities);
  pi_init.printTour();
  
  //STEP3 : 局所探索
  Tour pi_localOpt = searchLocalOpt(cities, LOCAL_SEARCH_METHOD, pi_init);
  pi_localOpt.setCost(cities);
  pi_localOpt.printTour();

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
 * ------ 今後の予定
 * 2. Nearest Neighbor 関数
 * 6. Usage 作成
 * 7. main関数 try-catchにする
 * 8. 各部分に throw 作成
 * (完了)9. cの性能低いrandを <random>の mt19937 の全域関数版にする
 *    mt19937 説明
 *    https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=devmachine&logNo=177685246
 *    https://redforce01.tistory.com/84
 *    mt19937 全域関数化
 *    https://stackoverflow.com/questions/33930182/using-same-random-number-generator-across-multiple-functions
 * 
 * yesyes
 */

  return 0;
}