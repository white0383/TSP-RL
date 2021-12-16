#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <map>
#include <cfloat> // DBL_MAX

#include "./model/Arguments.h"
#include "./RLmodel/LinearFittedQIteration.h"

#include "./solver/initial_solution/GenerateInitialSolution.h"
#include "./solver/local_search/SearchLocalOpt.h"

using namespace std;

const map<string, double> g_OPT {
  {"xqf131.tsp", 564.0},
  {"xqg237.tsp", 1019.0},
  {"pma343.tsp", 1368.0},
  {"xql662.tsp", 2513.0}
};

int main(int argc, char** argv){
  /************************************/
  /**** Set your test config here. ****/
  /************************************/
  vector<string> TSP_instances = {"xqf131.tsp", "xqg237.tsp", "pma343.tsp","xql662.tsp"};
  string TSP_instance = TSP_instances[3];//"xqf131.tsp";
  string initMethod = "RT";
  string localMethod = "SF2OPT";
  string termiCondi = "EPI";
  string greedyName = "PART";
  string weightsMethod = "UNIFM";

  unsigned int seed =  12080300;
  unsigned int T = 100;//100;
  unsigned int TMAX = 200;//200;
  unsigned int THETA = 50;
  unsigned int EPILMT = 50;
  unsigned int MMAX = 600;

  double gamma = 0.95;
  double alpha = 0.95;
  double secLMT = 180.0;
  double wPARA = 2.0;
  double greedyEPS = 0.1;
  double sf2optEPS = 0.0;

  vector<string> argSTR = {TSP_instance, initMethod, localMethod, termiCondi, "PART", weightsMethod};
  vector<unsigned int> argINT = {seed, T, TMAX,THETA,EPILMT,MMAX};
  vector<double> argREA = {gamma, alpha, secLMT, wPARA, greedyEPS, sf2optEPS};

  int testMAX = 1;
  //int tourNum = 1000;
  LinearFittedQIteration* LinQ;

/* compare Greedy algorithms: part, full, samp */
/* FULL experi 
  // FULL
  argSTR[4] = "FULL";

  vector<double> fullScosts;
  vector<double> fullCosts;
  vector<double> fullWeights;
  vector<unsigned int> fullEpis;

  for(int testIte = 0; testIte < testMAX; testIte++){
    cout << "start FULL " << testIte << endl;
    argINT[0] += 1; //increase seed
    Arguments tspArgs = Arguments(argSTR, argINT, argREA);
    LinQ = new LinearFittedQIteration(tspArgs);
    LinQ->learn(tspArgs);

    // begin something with model
    fullScosts.push_back(LinQ->bestDist);
    LinQ->bestTour.setCost(tspArgs.V);
    fullCosts.push_back(LinQ->bestTour.getCost());
    fullWeights = LinQ->weights;
    fullEpis.push_back(LinQ->epi);
    // end

    //free LinQ memory
    delete LinQ;
  }
*/
//PART experi   
  // PART
  argSTR[4] = "PART";

  vector<double> partScosts;
  vector<double> partCosts;
  vector<double> partWeights;
  vector<unsigned int> partEpis;

  for(int testIte = 0; testIte < testMAX; testIte++){
    cout << "start PART " << testIte << endl;
    argINT[0] += 1; //increase seed
    Arguments tspArgs = Arguments(argSTR, argINT, argREA);
    LinQ = new LinearFittedQIteration(tspArgs);
    LinQ->learn(tspArgs);

    // do something with model
    partScosts.push_back(LinQ->bestDist);
    LinQ->bestTour.setCost(tspArgs.V);
    partCosts.push_back(LinQ->bestTour.getCost());
    partWeights = LinQ->weights;
    partEpis.push_back(LinQ->epi);
    // dododo

    //free LinQ memory
    delete LinQ;
  }

/* SAMP experi 
  // SAMP
  argSTR[4] = "SAMP";

  vector<double> sampScosts;
  vector<double> sampCosts;
  vector<double> sampWeights;
  vector<unsigned int> sampEpis;

  for(int testIte = 0; testIte < testMAX; testIte++){
    cout << "start SAMP " << testIte << endl;
    argINT[0] += 1; //increase seed
    Arguments tspArgs = Arguments(argSTR, argINT, argREA);
    LinQ = new LinearFittedQIteration(tspArgs);
    LinQ->learn(tspArgs);

    // do something with model
    sampScosts.push_back(LinQ->bestDist);
    LinQ->bestTour.setCost(tspArgs.V);
    sampCosts.push_back(LinQ->bestTour.getCost());
    sampWeights=LinQ->weights;
    sampEpis.push_back(LinQ->epi);
    // dododo

    //free LinQ memory
    delete LinQ;
  }
*/


/* FULL print 
  cout << "FULL scores :: ";
  double fullMean;
  double fullBest = 100000000.0;
  for(auto foo : fullCosts){
    if(foo < fullBest) fullBest = foo;
    fullMean += foo;
    cout << foo << " ";
  }
  cout << endl;
  fullMean = fullMean / testMAX;
  cout << "FULL mean score :: " << fullMean << endl;
  cout << "FULL best score :: " << fullBest << endl;
  cout << "FULL ratio :: " << fullBest / XQF131OPT << endl;
  cout << "FULL weights :: " ;
  for(auto foo : fullWeights) cout << foo << " ";
  cout << endl << endl;
  cout << "FULL epis :: " ;
  for(auto foo : fullEpis) cout << foo << " ";
  cout << endl << endl;
*/

/* PART print
  cout << "PART scores :: ";
  double partMean;
  double partBest = 100000000.0;
  for(auto foo : partCosts){
    if(foo < partBest) partBest = foo;
    partMean += foo;
    cout << foo << " ";
  }
  cout << endl;
  partMean = partMean / testMAX;
  cout << "PART mean score :: " << partMean << endl;
  cout << "PART best score :: " << partBest << endl;
  cout << "PART ratio :: " << partBest / XQL662OPT << endl;
  cout << "PART weights :: ";
  for(auto foo : partWeights) cout << foo << " ";
  cout << endl << endl;
  cout << "PART epis :: " ;
  for(auto foo : partEpis) cout << foo << " ";
  cout << endl << endl;
*/


/* SAMP print 
  cout << "SAMP scores :: ";
  double sampMean;
  double sampBest = 100000000.0;
  for(auto foo : sampCosts){
    if(foo < sampBest) sampBest = foo;
    sampMean += foo;
    cout << foo << " ";
  }
  cout << endl;
  sampMean = sampMean / testMAX;
  cout << "SAMP mean score :: " << sampMean << endl;
  cout << "SAMP best score :: " << sampBest << endl;
  cout << "SAMP ratio :: " << sampBest / XQL662OPT << endl;
  cout << "SAMP weights :: ";
  for(auto foo : sampWeights) cout << foo << " ";
  cout << endl << endl;
  cout << "SAMP epis :: " ;
  for(auto foo : sampEpis) cout << foo << " ";
  cout << endl << endl;
*/


/* compare with RT, FI heuristics 
  LinearFittedQIteration* LinQ;
  Arguments* tspArgs;

  map<string, vector<double> > distsMapOur;
  map<string, vector<double> > timeMapOur;
  map<string, vector<double> > distsMapRT;
  map<string, vector<double> > timeMapRT;
  map<string, vector<double> > distsMapFI;
  map<string, vector<double> > timeMapFI;

  time_t totalStart = clock();
  for(string TSP_instance : TSP_instances){
    cout << "begin " << TSP_instance << endl;
    // Our Approach
    vector<string> argSTR = {TSP_instance, initMethod, localMethod, termiCondi, "PART", weightsMethod};
    vector<unsigned int> argINT = {seed, T, TMAX,THETA,EPILMT,MMAX};
    vector<double> argREA = {gamma, alpha, secLMT, wPARA, greedyEPS, sf2optEPS};

    vector<double> distVecOur;
    vector<double> timeVecOur;

    cout << "OUR" << endl;
    for(int i = 0; i< testMAX; i++){
      cout << i << endl;
      argINT[0] += 1; // increase seed
      tspArgs = new Arguments(argSTR, argINT, argREA);
      LinQ = new LinearFittedQIteration(*tspArgs);

      time_t startT = clock();
      LinQ->learn(*tspArgs);
      double spentTime = (double)(clock() - startT) / CLOCKS_PER_SEC;
      LinQ->bestTour.setCost(tspArgs->V);
      distVecOur.push_back(LinQ->bestTour.getCost());
      timeVecOur.push_back(spentTime);

      delete LinQ;
      delete tspArgs;
    }

    distsMapOur[TSP_instance] = distVecOur;
    timeMapOur[TSP_instance] = timeVecOur;
    
    // RT + 2opt
    cout << "RT" << endl;
    argSTR[1] = "RT";
    argSTR[2] = "F2OPT";

    vector<double> distVecRT;
    vector<double> timeVecRT;

    for(int i = 0; i< testMAX ; i++){
      cout << i << endl;
      argINT[0] += 1; // increase seed
      tspArgs = new Arguments(argSTR, argINT, argREA);

      time_t startT = clock();
      double best_dist_RT = DBL_MAX;

      for(int j = 0; j<tourNum ; j++){
        Tour pi_init = generateInitialSolution(*tspArgs);
        Tour pi_star = searchLocalOpt(*tspArgs, pi_init);
        pi_star.setCost(tspArgs->V);
        if(best_dist_RT > pi_star.getCost()) best_dist_RT = pi_star.getCost();
      }    

      double spentTime = (double)(clock() - startT) / CLOCKS_PER_SEC;
      distVecRT.push_back(best_dist_RT);
      timeVecRT.push_back(spentTime);

      delete tspArgs;
    }

    distsMapRT[TSP_instance] = distVecRT;
    timeMapRT[TSP_instance] = timeVecRT;

    // FI + 2opt
    argSTR[1] = "FI";
    argSTR[2] = "F2OPT";

    vector<double> distVecFI;
    vector<double> timeVecFI;

    for(int i = 0; i<testMAX ; i++){
      argINT[0] += 1; // increase seed
      tspArgs = new Arguments(argSTR, argINT, argREA);

      time_t startT = clock();
      double best_dist_FI = DBL_MAX;

      for(int j = 0; j<tourNum ; j++){
        //Tour pi_init = generateInitialSolution(*tspArgs);
        //Tour pi_star = searchLocalOpt(*tspArgs, pi_init);
        //pi_star.setCost(tspArgs->V);
        //if(best_dist_FI > pi_star.getCost()) best_dist_FI = pi_star.getCost();
      }    

      double spentTime = (double)(clock() - startT) / CLOCKS_PER_SEC;
      distVecFI.push_back(best_dist_FI);
      timeVecFI.push_back(spentTime);      

      delete tspArgs;
    }

    distsMapFI[TSP_instance] = distVecFI;
    timeMapFI[TSP_instance] = timeVecFI;

    seed = argINT[0];
  }
  double totalSpent = (double)(clock() - totalStart) / CLOCKS_PER_SEC;

  //print OUR
  cout << "======= OUR APPROACH =======" << endl; 
  for(string TSP_instance : TSP_instances){
    cout << "=== Instance : " << TSP_instance << " ===" << endl;

    double distMean = 0;
    double distBest = DBL_MAX;
    cout << "dist : ";
    for(double dist : distsMapOur.at(TSP_instance)){
      if(dist < distBest) distBest = dist;
      distMean += dist;
      cout << dist <<" ";
    }
    cout << endl;
    distMean = distMean / testMAX;
    cout << "mean dist : " << distMean <<endl;
    cout << "best dist : " << distBest <<endl;
    cout << "mean ratio : " << distMean / g_OPT.at(TSP_instance) << endl;
    cout << "best ratio : " << distBest / g_OPT.at(TSP_instance) << endl;

    double timeMean = 0;
    double timeBest = DBL_MAX;
    cout << "time : ";
    for(double time : timeMapOur.at(TSP_instance)){
      if(time < timeBest) timeBest = time;
      timeMean += time;
      cout << time << " ";
    }
    cout << endl;
    timeMean = timeMean / testMAX;
    cout << "mean time : " << timeMean <<endl;
    cout << "best time : " << timeBest <<endl;
  }

  //print RT
  cout << "======= Random Tour =======" << endl; 
  for(string TSP_instance : TSP_instances){
    cout << "=== Instance : " << TSP_instance << " ===" << endl;

    double distMean = 0;
    double distBest = DBL_MAX;
    cout << "dist : ";
    for(double dist : distsMapRT.at(TSP_instance)){
      if(dist < distBest) distBest = dist;
      distMean += dist;
      cout << dist <<" ";
    }
    cout << endl;
    distMean = distMean / testMAX;
    cout << "mean dist : " << distMean <<endl;
    cout << "best dist : " << distBest <<endl;
    cout << "mean ratio : " << distMean / g_OPT.at(TSP_instance) << endl;
    cout << "best ratio : " << distBest / g_OPT.at(TSP_instance) << endl;

    double timeMean = 0;
    double timeBest = DBL_MAX;
    cout << "time : ";
    for(double time : timeMapRT.at(TSP_instance)){
      if(time < timeBest) timeBest = time;
      timeMean += time;
      cout << time << " ";
    }
    cout << endl;
    timeMean = timeMean / testMAX;
    cout << "mean time : " << timeMean <<endl;
    cout << "best time : " << timeBest <<endl;
  }
  //print FI
  cout << "======= Farthest Insertion =======" << endl; 
  for(string TSP_instance : TSP_instances){
    cout << "=== Instance : " << TSP_instance << " ===" << endl;

    double distMean = 0;
    double distBest = DBL_MAX;
    cout << "dist : ";
    for(double dist : distsMapFI.at(TSP_instance)){
      if(dist < distBest) distBest = dist;
      distMean += dist;
      cout << dist <<" ";
    }
    cout << endl;
    distMean = distMean / testMAX;
    cout << "mean dist : " << distMean <<endl;
    cout << "best dist : " << distBest <<endl;
    cout << "mean ratio : " << distMean / g_OPT.at(TSP_instance) << endl;
    cout << "best ratio : " << distBest / g_OPT.at(TSP_instance) << endl;

    double timeMean = 0;
    double timeBest = DBL_MAX;
    cout << "time : ";
    for(double time : timeMapFI.at(TSP_instance)){
      if(time < timeBest) timeBest = time;
      timeMean += time;
      cout << time << " ";
    }
    cout << endl;
    timeMean = timeMean / testMAX;
    cout << "mean time : " << timeMean <<endl;
    cout << "best time : " << timeBest <<endl;
  }
*/
  //########### Print Experiment infomation ###############

  cout << "======= Experiment infomation =========" << endl;
  cout << "TSP_instance : " << TSP_instance << endl;
  cout << "initMethod : " << initMethod << endl;
  cout << "localMethod : " << localMethod << endl;
  cout << "termiCondi : " << termiCondi << endl;
  //cout << "greedyName : " <<  << endl;
  cout << "weightsMethod : "<< weightsMethod << endl;

  cout << "seed : " << seed << endl;
  cout << "T : " << T << endl;
  cout << "TMAX : " << TMAX << endl;
  cout << "THETA : " << THETA << endl;
  cout << "EPILMT : " << EPILMT << endl;
  cout << "MMAX : " << MMAX << endl;

  cout << "gamma : " << gamma << endl;
  cout << "alpha : " << alpha << endl;
  cout << "secLMT : " << secLMT<< endl;
  cout << "wPARA : " << wPARA<< endl;
  cout << "greedyEPS : " << greedyEPS<< endl;
  cout << "sf2optEPS : " << sf2optEPS<< endl;

  cout << "testMAX : " << testMAX << endl;

  //cout << "totalSpent sec : " << totalSpent << endl;

  //cout << "MEMO : compare with other heuristics for 1208 zemi" << endl;
  cout << "MEMO : descriptor kigou check for 1210 zemi" << endl;
  cout << "finish experi 21/12/10 14:37 Lee Kunwoo" << endl;

  return 0;
}
