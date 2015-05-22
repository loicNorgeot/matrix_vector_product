#include "matrix.h"
#include "csrmatrix.h"
#include "vector.h"
#include "omp.h"
#include <string>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

//PROTOTYPES
Vector GC(const CSRMatrix& A; const Vector& b, const int nP);
double scal(const Vector& V1, const Vector& V2);


Vector GC(const CSRMatrix& A; const Vector& b, const int nP){
  //PARAMETRES DU CALCUL
  int nI = 100;
  double eps = 1e-5;

  //INITIALISATION
  int nR = A.GetNumberOfRows();
  Vector X(nR);
  Vector g(nR);
  Vector h(nR);
  for (int i = 0 ; i < nR ; i++){
    X[i] = 1;
  }
  g = parMult(A,X,nP) - b;
  h = -g;

  //ALGORITHME DU GRADIENT CONJUGUE
  for (int i = 0 ; i < nI ; i++){
    Vector Ah(nR);
    Ah = parMult(A,h,np);
    double rau = scal(g, h) / scal(h, Ah);
    X += rau * h;
    Vector g_new(nR);
    g_new = g + rau * Ah;
    double gamma = scal(g_new, g_new) / scal(g, g);
    g = g_new;
    h = -g + gamma * h;
    if( scal(g,g) < eps ){
      break;
    }
  }
  return X;
}


double scal(const Vector& V1, const Vector& V2){
  assert(V1.GetSize() == V2.GetSize());
  n = V1.GetSize();
  double S = 0;
  for (int i = 0 ; i < n ; i++){
    S += V1.Read(i) * V2.Read(i);
  }
  return S;
}
  
