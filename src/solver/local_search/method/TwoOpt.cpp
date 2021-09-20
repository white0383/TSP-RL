#include <cstdlib> // should be replaced to <random>'s mt19937
#include <iostream>
#include <vector>
#include <cfloat> // DBL_MAX

#include "TwoOpt.h"
#include "../../../model/Distance.h"
#include "../../../model/Node.h"


using namespace std;
/**
 * Memo : written 20210920 14:56
 * 多分3種類の2-optアルゴリズムが存在すると思われる
 * 1. 全ての(i,j)の組み合わせをスキャンして
 *    最もコストが改善されるものを選び、スワップする。
 *    これを改善が発見されないまで繰り返す。
 * 2. 全ての(i,j)をスキャンする途中で
 *    改善が発見されたらスワップし、
 *    その即時最初に戻る。
 *    これを改善が発見されないまで繰り返す
 * 3. 全ての(i,j)をスキャンする途中で
 *    改善が発見されたらスワップし、
 *    最初に戻らず最後までスキャンとスワップを行う。
 *    これを改善が発見されないまで繰り返す
 * 
 * ここでは 3. の方法を実装したが、
 * この３つの方法を比較することも意味があると思われる。
 */

/**
 * 2-opt is like below : (i) represent ith node on tour
 * 
 * -> (i-1) ->   <- (j) <-     swap    -> (i-1) -> (j) -> 
 *             ×               ===>
 * <- (j+1) <-   -> (i) ->             <- (j+1) <- (i) <-
 * 
 * dist(a,b) represent distance between ath node and bth node on tour.
 * Swap occurs when dist(i-1, i) + dist(j, j+1) > dist(i-1,j) + dist(i,j+1)
 * 
 * To make nodes name simple,
 * (i-1) is node A
 * (i)   is node B
 * (j)   is node C
 * (j+1) is node D
 * And
 * Swap when dist(A,B) + dist(C,D) > dist(A,C) + dist(B,D)
 */
bool isNewTourShorter(vector<int> pi, const Graph& g, int i, int j){
  // node's index is always 1 smaller than city's index
  Node A = g.nodes.at(pi[i-1] - 1);
  Node B = g.nodes.at(pi[i] - 1);
  Node C = g.nodes.at(pi[j] - 1);
  Node D = g.nodes.at(pi[j+1] - 1);

  double distBeforeSwap = dist(A,B) + dist(C,D);
  double distAfterSwap = dist(A,C) + dist(B,D);
  
  if(distBeforeSwap > distAfterSwap){
    return true;
  }else{
    return false;
  }
}

/**
 * "Swap" operator in 2-opt is like below (https://en.wikipedia.org/wiki/2-opt)
 * 
 *  input vector pi, int i and j
 *        (a) represent pi[a]
 *  1. take (start) to (i-1) and add them in order to new_pi
 *  2. take (i) to (j) and add them in reverse order to new_pi
 *  3. take (j+1) to (end) and add them in order to new_pi
 *  return new_pi
 */
vector<int> swapTwoOpt(vector<int> pi, int i, int j){
  vector<int> new_pi;
  int cityNum = pi.size();
  new_pi.reserve(cityNum);

  int it;
  int reverse_it;
  for(it = 0; it<i;it++){
    new_pi.push_back(pi[it]);
  }

  /**
   * map it to reverse_it = f(it)
   * 
   * it    : i, i+1, i+2, ... , j-1, j
   * r_it  : j, j-1, ..., i+2, i+1, i
   * 
   * then f(it) = j - (it - i)
   */
  for(it = i; it<j+1;it++){
    reverse_it = j - (it - i);
    new_pi.push_back(pi[reverse_it]);
  }

  for(it = j+1; it<cityNum; it++){
    new_pi.push_back(pi[it]);
  }
  return new_pi;
}

Tour TwoOpt(const Graph& g, Tour& pi){
  vector<int> pi_order = pi.getTour();
  int cityNum = pi.getSize();
  bool improved = true;

  int debugPara = 0;
  while(improved){
    improved = false;
    for(int i=1; i < cityNum-1 ; i++){
      for(int j=i+2; j < cityNum ; j++){
        //exception handling. adjacent edge
        if((i == 1) && (j == cityNum-1)) continue;

        if(isNewTourShorter(pi_order, g, i ,j)) {
          pi_order = swapTwoOpt(pi_order, i, j);
          improved = true;
        }
      }
    }
  }

  pi_order.pop_back(); // cut tail
  
  Tour pi_star(pi_order,g);

  return pi_star;
}