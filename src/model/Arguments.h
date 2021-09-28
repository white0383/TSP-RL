#ifndef TSP_ARGUMENTS_H
#define TSP_ARGUMENTS_H

#include <string>
#include <vector>
#include "../model/Graph.h"

/**
 * 한가지 강화학습에 필요한 모든 전달인자(argument)를 하나의 객체로 다룬다.
 * 사용시 "반드시" const Arguments& foo 의 형태로 함수에 전달되며,
 * 각 맴버 변수를 public으로 선언해서 . 연산자로 접근 가능하게 한다.
 */

class Arguments {
  public : 
    //Input arguments
    string TSP_DATA_FILE ;
    string INIT_SOLUTION_METHOD ;
    string LOCAL_SEARCH_METHOD ;
    unsigned int SEED ;
    unsigned int T ;
    unsigned int TMAX ;
    unsigned int THETA ;
    unsigned int ITERATION_LIMIT ; 
    double GAMMA ;
    double ALPHA ;
    double TIME_LIMIT ;
    //Arguments that are calculated 
    unsigned int K ;
    unsigned int KSMP ;
    unsigned int OMEGA ;
    Graph V;
  public :
    Arguments();

    Arguments(vector<string>& stringArgs, vector<unsigned int>& integerArgs, vector<double>& realArgs);
};

#endif // TSP_ARGUMENTS_H