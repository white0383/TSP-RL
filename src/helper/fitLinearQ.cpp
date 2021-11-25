#include <vector>
#include "./fitLinearQ.h"
#include "/usr/local/include/lapacke.h"
#include <stdlib.h>

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
  lda = K+1;
  ldb = 1;

  info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', T, K+1, nrhs,A,lda,b,ldb);

  vector<double> rst_w(K+1);
  for(int i=0;i<K+1;i++){
    rst_w[i] = b[i];
  }

  delete[]A;
  delete[]b;

  return rst_w;
}
