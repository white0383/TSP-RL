#include <vector>
#include "./fitLinearQ.h"
#include "/usr/local/include/lapacke.h"
#include <stdlib.h>
//tmp
#include <iostream>

using namespace std;

vector<double> fitLinearQ(const vector< vector<double> >& X, const vector<double>& rho){
  int T = rho.size();
  int K = X[0].size();
  int bSize = (T > (K+1)) ? T : K+1;

  double *A = new double[(K+1) * T];
  double *b = new double[bSize];

  for(int i=0;i<T;i++){
    //set A's ist column
    A[i*(K+1)] = 1;
    for(int j=1;j<K+1;j++){
      A[i * (K+1) + j] = X[i][j];
    }
    //set b's ist element
    b[i] = rho[i];
  }

  lapack_int info,nrhs,lda,ldb;

  nrhs = 1;
  lda = K+1;//T;//K+1;// T
  ldb = K+1;//K+1;

  //old
  //info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', T, K+1, nrhs,A,lda,b,ldb);
  //new
  info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'T', T, K+1, nrhs,A,lda,b,ldb);

  vector<double> rst_w(K+1);
  for(int i=0;i<K+1;i++){
    rst_w[i] = b[i];
  }

  //tmp
  double errSum=0;
  double err=0;
  double err_tmp=0;
  for(int i=0;i<T;i++){
    err_tmp = rho[i];
    for(int j=0;j<K+1;j++){
      err_tmp -= rst_w[j] * A[i*(K+1) +j];
    }
    err = (err_tmp * err_tmp);
    errSum += err;
  }

  cout << "errSum :" << errSum << endl;
  double errOpt = errSum;

  //tmp2 
  vector<double> rst_ww(K+1);
  for(int i=0;i<K+1;i++){
    rst_ww[i] = rst_w[i] + 1.0;
  }

  errSum=0;
  err=0;
  err_tmp=0;
  for(int i=0;i<T;i++){
    err_tmp = rho[i];
    for(int j=0;j<K+1;j++){
      err_tmp -= rst_ww[j] * A[i*(K+1) +j];
    }
    err = (err_tmp * err_tmp);
    errSum += err;
  }

  cout << "errSum2 :" << errSum << endl;

  bool checkOpt = (errOpt < errSum);
  if(checkOpt) cout << "OPT" << endl;
  if(!checkOpt) cout << "NOT OPT" << endl;
  double ratio = errOpt / errSum;
  cout << "ratio : " << ratio<< endl;

  delete[]A;
  delete[]b;

  return rst_w;
}
