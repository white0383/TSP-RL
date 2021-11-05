#include <vector>
#include <string>
#include <iostream>
#include "./Arguments.h"
#include "../model/Graph.h"
#include "../helper/FileIOHelper.h"
#include "../helper/mt19937ar.h"

using namespace std;

#define STRINUM 5
#define INTENUM 5
#define REALNUM 5

bool verifyInputVectorLength(vector<string>& stringArgs, vector<unsigned int>& integerArgs, vector<double>& realArgs){
  return (stringArgs.size() == STRINUM) && (integerArgs.size() == INTENUM) && (realArgs.size() == REALNUM);
}

Arguments::Arguments() {
  cout << "ERROR : Empty Arguments constructor is not allowed" << endl;
  exit(1);
}

Arguments::Arguments(vector<string>& stringArgs, vector<unsigned int>& integerArgs, vector<double>& realArgs){
  if(verifyInputVectorLength(stringArgs,integerArgs,realArgs)){
    this->TSP_DATA_FILE = stringArgs[0];
    this->INIT_SOLUTION_METHOD = stringArgs[1];
    this->LOCAL_SEARCH_METHOD = stringArgs[2];
    this->TERMINATE_METHOD = stringArgs[3];
    this->ACTION_GREEDY_METHOD = stringArgs[4];

    this->SEED = integerArgs[0];
    this->T = integerArgs[1];
    this->TMAX = integerArgs[2];
    this->THETA = integerArgs[3];
    this->EPI_LIMIT = integerArgs[4];

    this->GAMMA = realArgs[0];
    this->ALPHA = realArgs[1];
    this->SEC_LIMIT = realArgs[2];
    this->WEIGHT_INTERVAL = realArgs[3];
    this->EPS = realArgs[4];
    //Exception Check
    if((this->EPS < 0.0) || (this->EPS > 1.0)){
      cout <<"ERROR : Arguments constructor improper input" << endl;
      cout << "5th real argument EPS have to be included in [0,1]" << endl;
      exit(1);
    }

    init_genrand(SEED);

    vector< vector <int> > nodes = readTSPFile(this->TSP_DATA_FILE);
    this->V = Graph(nodes);
    this->KSMP = min(this->V.getN(),100);
    this->OMEGA = min(this->V.getN(),100);
    this->K = 3+ this->KSMP + this->OMEGA + 6;
    this->M = min(this->V.getN(),100);

  } else {
    //後でUSAGE.txtを出力するように変える
    cout <<"ERROR : improper input vectors' length" << endl;
    cout <<"please check your input parameter has" << endl;
    cout << STRINUM << " strings " << endl;
    cout << INTENUM << " unsigned int" << endl;
    cout << REALNUM << " double " << endl;
    exit(1);
  }
}