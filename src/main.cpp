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
  vector<string> tmpSTR = {"xqg237.tsp", "FI", "F2OPT", "EPI", "FULL", "UNIFM"};
  vector<unsigned int> tmpINT = {1235, 100, 500, 10, 1000, 1000};
  vector<double> tmpREA = {0.95, 0.95, 123.1, 100, 0.5};
  Arguments tmpArgs = Arguments(tmpSTR, tmpINT, tmpREA);

  //Define LinQ model
  LinearFittedQIteration* tmpLinQModel = new LinearFittedQIteration(tmpArgs);

  //Learn Q-function
  //tmpLinQModel->learn(tmpArgs);

  //do something with constructed model
  cout << "hello~~" << endl;
  
  //Free memory
  delete tmpLinQModel;

  return 0;
}
