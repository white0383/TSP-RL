/**
 * Description of methods' name
 * 
 * Details in below
 * 
 * 1 2OPT : 2-opt -> search neighborhood in sequence
 * 
 * ----- not yet implemented (written in 2021 09 19 16:52)
 * 2 R2OPT : random 2-opt -> search neighborhood randomly
 * 3 3OPT : 3-opt
 * 4 LK : Lin–Kernighan method
 */

#include "SearchLocalOpt.h"
#include <iostream>
#include <string>

#include "./method/TwoOpt.h"
//#include "./method/RandomTwoOpt.h"
//#include "./method/ThreeOpt.h"
//#include "./method/LinKernighan.h"
using namespace std;

Tour searchLocalOpt(const Graph& g, const string& method, Tour& pi){
  /**
   * There are several ways to avoid if-else statement like below,
   * but this is much easier to read and manage.
   * 
   * Using string in switch statement makes code longer and hard to read.
   * Moreover, switch statement is not that faster than this short if-else chain (in my opinion)
   */
  if(!(pi.isCompleteTour(g))){
    cout << "ERROR : Tour solution is not a complete tour" << endl;
    exit(1);
  }

  
  if(method.compare("2OPT") == 0) {
    return TwoOpt(g, pi);
  //######### BEGIN: NOT YET IMPLEMENTED
  //}else if (method.compare("R2OPT") == 0){

  //}else if (method.compare("3OPT") == 0){

  //}else if (method.compare("LK") == 0){
  //######### END: NOT YET IMPLEMENTED
  }else {
    //後でthrow文で書き直す
    cout << "ERROR : invalid Local Search method name" << endl;
    exit(1);
  }
}