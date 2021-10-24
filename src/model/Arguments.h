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
    string TSP_DATA_FILE ; // file name of tsp instance
    string INIT_SOLUTION_METHOD ; // name of init solution generator
    string LOCAL_SEARCH_METHOD ; // name of local-opt searcher
    string TERMINATE_METHOD; // name of termination method (time or episode)

    unsigned int SEED ; // seed of rng
    unsigned int T ; // length of Replay Buffer
    unsigned int TMAX ; // length of Samples
    unsigned int THETA ; // parameter in reward function
    unsigned int EPI_LIMIT ; // Maximum value of episode number

    double GAMMA ; // parameter for calculating target values
    double ALPHA ; // parameter in reward function
    double SEC_LIMIT ; // Maximum value of second
    double WEIGHT_INTERVAL ; // Interval of initial weight vector

    //Arguments that are calculated 
    unsigned int K ; // lenght of feature vector and weight vector(minus 1)
    unsigned int KSMP ; // the number of sample node in feature mapping (state part)
    unsigned int OMEGA ; // the number of largest sum of edge weight in feature mapping (state part)

    Graph V; // set of nodes
  public :
    Arguments();

    Arguments(vector<string>& stringArgs, vector<unsigned int>& integerArgs, vector<double>& realArgs);
};

#endif // TSP_ARGUMENTS_H