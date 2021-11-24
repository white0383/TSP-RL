#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "./model/Arguments.h"
#include "./RLmodel/LinearFittedQIteration.h"

using namespace std;

int main(int argc, char** argv){
  /************************************/
  /**** Set your test config here. ****/
  /************************************/
  vector<string> tmpSTR = {"pia3056.tsp", "RT", "SF2OPT", "EPI", "PART", "GAUSS"};
  //                      SEED, T, TMAX, THETA, EPI_LIMIT, MMAX
  vector<unsigned int> tmpINT = {1235, 2, 100, 10, 100, 1000};
  //                      GAMMA, ALPHA, SEC_LIMIT, WEIGHTPARA, EPS, EPS_SF2OPT
  vector<double> tmpREA = {0.95, 0.95, 123.1, 1, 0.1, 0};
  Arguments tmpArgs = Arguments(tmpSTR, tmpINT, tmpREA);

  //Define LinQ model
  LinearFittedQIteration* tmpLinQModel = new LinearFittedQIteration(tmpArgs);

  //Learn Q-function
  time_t startT = clock();
  tmpLinQModel->learn(tmpArgs);
  time_t endT = clock();
  cout << "spend time : " << (double)(endT - startT)/CLOCKS_PER_SEC << endl;

  //cout << "dist q : ";
  //for(auto foo : tmpLinQModel->distQueue) cout << foo << " ";
  //cout << endl;
  

  //do something with constructed model
  cout << "End Learning" << endl;
  
  //Free memory
  delete tmpLinQModel;

  return 0;
}
