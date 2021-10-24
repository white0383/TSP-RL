#include <iostream>
#include <vector>
#include <utility> // std::pair
#include <cstdlib> // abs(int num)
#include <numeric> // std::iota
#include <random> // std::mt19937
#include <algorithm> // std::shuffle

#include "FastTwoOpt.h"
#include "../../../model/Distance.h"
#include "../../../model/Node.h"
#include "../../../helper/mt19937ar.h"

using namespace std;

void swapFastTwoOptIn(vector<int>& pi, vector<int>& pi_inv, int i, int j){
  cout << "BEFORE : " ; 
  cout << "i : " << i << " j : " << j << endl;
  //### before
  cout << "pi : ";
  for(int foo=1;foo<pi.size();foo++) cout << pi[foo] << " ";
  cout << endl;
  cout << "pi_inv : ";
  for(int foo=1;foo<pi_inv.size();foo++) cout << pi_inv[foo] << " ";
  cout << endl;


  vector<int> pi_rev_tmp(j-i);

  //copy from pi[i+1] to pi[j] in reverse
  for(int it=0;it<j-i;it++){
    pi_rev_tmp[it] = pi[-it + j];
  }

  //reverse from pi[i+1] to pi[j]
  int pi_tmp_index=0;
  for(int it=0;it<j-i;it++){
    pi_tmp_index = it+i+1;
    pi[pi_tmp_index] = pi_rev_tmp[it];
    pi_inv[pi[pi_tmp_index]] = pi_tmp_index;
  }

  //### after
  cout << "AFTER  : " ; 
  cout << "i : " << i << " j : " << j << endl;
  //### before
  cout << "pi : ";
  for(int foo=1;foo<pi.size();foo++) cout << pi[foo] << " ";
  cout << endl;
  cout << "pi_inv : ";
  for(int foo=1;foo<pi_inv.size();foo++) cout << pi_inv[foo] << " ";
  cout << endl;
}

void swapFastTwoOptEx(vector<int>& pi, vector<int>& pi_inv, int i, int j){
  int n = pi.size();
  vector<int> pi_rev_tmp(n-j+i);

  //copy from pi[j+i] to pi[n] and from pi[1] to pi[i] in reverse
  for(int it=0;it<i;it++){
    pi_rev_tmp[it] = pi[-it + i];
  }
  for(int it=i;it < n-j+i;it++){
    pi_rev_tmp[it] = pi[-it + n + i];
  }

  //reverse from pi[j+i] to pi[n] and from pi[1] to pi[i]
  int pi_tmp_index=0;
  for(int it=0;it<n-j;it++){
    pi_tmp_index = it+j+1;
    pi[pi_tmp_index] = pi_rev_tmp[it];
    pi_inv[pi[pi_tmp_index]] = pi_tmp_index;
  }
  for(int it=n-j;it<n-j+i;it++){
    pi_tmp_index = it+j+1-n;
    pi[pi_tmp_index] = pi_rev_tmp[it];
    pi_inv[pi[pi_tmp_index]] = pi_tmp_index;
  }
}

void swapFastTwoOpt(vector<int>& pi, vector<int>& pi_inv, int i, int j){
  /*
  if(i > j){
    swapFastTwoOpt(pi, pi_inv, j, i);
  } else if(2*(j-i) <= pi.size()-1){
    swapFastTwoOptIn(pi, pi_inv, i, j);
  } else {
    swapFastTwoOptEx(pi, pi_inv, i, j);
  }
  */
  if(i > j){
    swapFastTwoOpt(pi, pi_inv, j, i);
  } else {
    swapFastTwoOptIn(pi, pi_inv, i, j);
  }
}

Tour fastTwoOpt(const Graph& g, Tour& pi){
  int cityNum = pi.getSize();
  bool improved = true;

  vector<int> pi_vec = pi.getTour();
  pi_vec.insert(pi_vec.begin(),0);

  vector<int> pi_vec_inv(cityNum+1);
  for(int i=1;i<cityNum+1;i++){
    pi_vec_inv[pi_vec[i]] = i;
  }

  // i_vec initialized -> {1,2, ... , n-1, n}
  vector<int> i_vec(cityNum);
  iota(i_vec.begin(), i_vec.end(), 1);

  // *_order : 巡回路の何番目か
  // *_index : 都市の番号, 名前
  // *_node  : ノードそのもの
  while(improved){
    // reset "improved"
    improved = false;
    cout << endl << "######$while begin$######" << endl;

    // shuffle "i_vec"
    mt19937 mtRand(genrand_int32());
    shuffle(i_vec.begin(), i_vec.end(),mtRand);

    for(int i_order : i_vec){
      Node p_node = g.nodes.at(pi_vec[i_order]);
      Node pp_node;
      if(i_order != cityNum){
        pp_node = g.nodes.at(pi_vec[i_order+1]);
      } else {
        cout << pi_vec[i_order] << " ||| " << pi_vec[1] << endl;
        pp_node = g.nodes.at(pi_vec[1]);
      }

      for(int iC = 1; iC < cityNum ; iC++){
        int j_order = pi_vec_inv.at(g.distOrder[p_node.index][iC].first);
        Node q_node = g.nodes.at(pi_vec[j_order]);
        Node qp_node;
        if(j_order != cityNum){
          qp_node = g.nodes.at(pi_vec[j_order+1]);
        } else {
          cout << pi_vec[j_order] << " ||| " << pi_vec[1] << endl;
          qp_node = g.nodes.at(pi_vec[1]);
        }

        if (abs(i_order - j_order) > 1){
          int dist_p_q = distSquare(p_node, q_node);
          int dist_p_pp = distSquare(p_node, pp_node);
          if (dist_p_q < dist_p_pp) {
            int dist_pp_qp = distSquare(pp_node, qp_node);
            int dist_q_qp = distSquare(q_node, qp_node);

            /*
            int tmpFront = dist_p_q + dist_pp_qp;
            int tmpRear = dist_p_pp + dist_q_qp;
            cout << "p_node : " ;
            p_node.printNode();
            cout << "pp_node : " ;
            pp_node.printNode();
            cout << "q_node : " ;
            q_node.printNode();
            cout << "qp_node : " ;
            qp_node.printNode();
            cout << "Front : " << tmpFront << " Rear : " << tmpRear << endl;
            cout << "i_order : " << i_order << " j_order : " << j_order << endl;
            */

            if((dist_p_q + dist_pp_qp) < (dist_p_pp + dist_q_qp)){
              cout << "upup" << endl;
              int tmpFront = dist_p_q + dist_pp_qp;
              int tmpRear = dist_p_pp + dist_q_qp;
              cout << "p_node : " ;
              p_node.printNode();
              cout << "pp_node : " ;
              pp_node.printNode();
              cout << "q_node : " ;
              q_node.printNode();
              cout << "qp_node : " ;
              qp_node.printNode();
              cout << "Front : " << tmpFront << " Rear : " << tmpRear << endl;
              cout << "i_order : " << i_order << " j_order : " << j_order << endl;

              swapFastTwoOpt(pi_vec, pi_vec_inv,i_order, j_order);
              improved = true;
              cout << "break 1" << endl;
              break;
            }
          } else {
            //cout << "break 2" << endl;
            break;
          }
        }
      }
      if (improved) {
        cout << "break 3" << endl;
        break;
      }

      for(int iCp=1;iCp < cityNum ; iCp++){
        int jp_order = pi_vec_inv.at(g.distOrder[pp_node.index][iCp].first);
        int j_order =jp_order - 1;

        Node qp_node = g.nodes.at(pi_vec[jp_order]);
        Node q_node = g.nodes.at(pi_vec[j_order]);

        if (abs(i_order - j_order) > 1){
          int dist_pp_qp = distSquare(pp_node, qp_node);
          int dist_p_pp = distSquare(p_node, pp_node);
          if (dist_pp_qp < dist_p_pp) {
            int dist_p_q = distSquare(p_node, q_node);
            int dist_q_qp = distSquare(q_node, qp_node);

            /*
            int tmpFront = dist_p_q + dist_pp_qp;
            int tmpRear = dist_p_pp + dist_q_qp;
            cout << "p_node : " ;
            p_node.printNode();
            cout << "pp_node : " ;
            pp_node.printNode();
            cout << "q_node : " ;
            q_node.printNode();
            cout << "qp_node : " ;
            qp_node.printNode();
            cout << "Front : " << tmpFront << " Rear : " << tmpRear << endl;
            cout << "i_order : " << i_order << " j_order : " << j_order << endl;
            */

            if((dist_p_q + dist_pp_qp) < (dist_p_pp + dist_q_qp)){
                
              cout << "down" << endl;
              int tmpFront = dist_p_q + dist_pp_qp;
              int tmpRear = dist_p_pp + dist_q_qp;
              cout << "p_node : " ;
              p_node.printNode();
              cout << "pp_node : " ;
              pp_node.printNode();
              cout << "q_node : " ;
              q_node.printNode();
              cout << "qp_node : " ;
              qp_node.printNode();
              cout << "Front : " << tmpFront << " Rear : " << tmpRear << endl;
              cout << "i_order : " << i_order << " j_order : " << j_order << endl;

              swapFastTwoOpt(pi_vec, pi_vec_inv,i_order, j_order);
              improved = true;
              cout << "break 4" << endl;
              break;
            }
          } else {
            //cout << "break 5" << endl;
            break;
          }
        }
      }
      if (improved){
        cout << "break 6" << endl;
        break;
      }

    }
  }
  //cut head of pi_vec (0-th member)
  pi_vec.erase(pi_vec.begin());
  Tour pi_star(pi_vec,g);
  pi_star.setThisIsLocalOpt();

  return pi_star;
}