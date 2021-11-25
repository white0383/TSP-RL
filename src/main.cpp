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
  string TSP_instance = "xqf131.tsp";

  vector<string> tmpSTR = {TSP_instance, "FI", "SF2OPT", "EPI", "PART", "GAUSS"};
  //                      SEED, T, TMAX, THETA, EPI_LIMIT, MMAX
  vector<unsigned int> tmpINT = {1126, 100, 200, 10, 100, 1000};
  //                      GAMMA, ALPHA, SEC_LIMIT, WEIGHTPARA, EPS, EPS_SF2OPT
  vector<double> tmpREA = {0.95, 0.95, 123.1, 1, 0.1, 0};
  Arguments tmpArgs = Arguments(tmpSTR, tmpINT, tmpREA);

  //Define LinQ model
  LinearFittedQIteration* tmpLinQModel = new LinearFittedQIteration(tmpArgs);

  //Learn Q-function
  time_t startT = clock();
  tmpLinQModel->learn(tmpArgs);
  time_t endT = clock();
  double partSec = (double)(endT - startT) / CLOCKS_PER_SEC;
  tmpLinQModel->bestTour.setCost(tmpArgs.V);
  double partScore = tmpLinQModel->bestTour.getCost();

  //do something with constructed model
  cout << "End Learning" << endl;
  
  //Free memory
  delete tmpLinQModel;

  
  vector<string> tmpSTR2 = {TSP_instance, "FI", "SF2OPT", "EPI", "FULL", "GAUSS"};
  vector<string> tmpSTR3 = {TSP_instance, "FI", "SF2OPT", "EPI", "SAMP", "GAUSS"};

  Arguments tmpArgs2 = Arguments(tmpSTR2, tmpINT, tmpREA);
  Arguments tmpArgs3 = Arguments(tmpSTR3, tmpINT, tmpREA);

  tmpLinQModel = new LinearFittedQIteration(tmpArgs2);
  startT = clock();
  
  tmpLinQModel->learn(tmpArgs);
  endT = clock();
  double fullSec = (double)(endT - startT) / CLOCKS_PER_SEC;  
  tmpLinQModel->bestTour.setCost(tmpArgs2.V);
  double fullScore = tmpLinQModel->bestTour.getCost();
  delete tmpLinQModel;

  tmpLinQModel = new LinearFittedQIteration(tmpArgs3);
  startT = clock();
  tmpLinQModel->learn(tmpArgs);
  endT = clock();
  double sampSec = (double)(endT - startT) / CLOCKS_PER_SEC;  
  tmpLinQModel->bestTour.setCost(tmpArgs3.V);
  double sampScore = tmpLinQModel->bestTour.getCost();
  delete tmpLinQModel;

  cout << "steps : " << tmpINT[2] * tmpINT[4] << endl;
  cout << "PART : " << partSec << " sec" << endl; 
  cout << "FULL : " << fullSec << " sec" << endl;
  cout << "SAMP : " << sampSec << " sec" << endl;
  cout << "##### SCORE #####" << endl;
  cout << "PART : " << partScore << endl; 
  cout << "FULL : " << fullScore << endl;
  cout << "SAMP : " << sampScore << endl;

  return 0;
}
