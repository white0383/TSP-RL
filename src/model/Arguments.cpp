#include <vector>
#include <string>
#include <iostream>
#include "./Arguments.h"
#include "../model/Graph.h"
#include "../helper/FileIOHelper.h"
#include "../helper/mt19937ar.h"

using namespace std;

#define STRINUM 4
#define INTENUM 5
#define REALNUM 3

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

    this->SEED = integerArgs[0];
    this->T = integerArgs[1];
    this->TMAX = integerArgs[2];
    this->THETA = integerArgs[3];
    this->ITERATION_LIMIT = integerArgs[4];

    this->GAMMA = realArgs[0];
    this->ALPHA = realArgs[1];
    this->TIME_LIMIT = realArgs[2];

    init_genrand(SEED);

    vector< vector <int> > nodes = readTSPFile(this->TSP_DATA_FILE);
    this->V = Graph(nodes);
    this->KSMP = min(this->V.getNodesSize(),100);
    this->OMEGA = min(this->V.getNodesSize(),100);
    this->K = 3+ this->KSMP + this->OMEGA + 6;

  } else {
    //後でUSAGE.txtを出力するように変える
    cout <<"ERROR : improper input parameter vector's length" << endl;
    cout <<"please check your input parameter has" << endl;
    cout << STRINUM << " strings " << endl;
    cout << INTENUM << " unsigned int" << endl;
    cout << REALNUM << " double " << endl;
    exit(1);
  }
}