#include "csrmatrix.h"
#include "vector.h"
#include "gc.h"
#include "matrix.h"

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

double scal(const Vector& V1,
	    const Vector& V2){
  int n = V1.GetSize();
  double S = 0;
  for (int i = 0 ; i < n ; i++){
    S += V1.Read(i) * V2.Read(i);
  }
  return S;
}

double scalC(const Vector& V1,
	     const Vector& V2,
	     const Matrix& C){
  Vector V = C * V1;
  int n = V.GetSize();
  double S = 0;
  for (int i = 0 ; i < n ; i++){
    S += V.Read(i) * V2.Read(i);
  }
  return S;
}

double scalC(const Vector& V1,
	     const Vector& V2,
	     const Vector& C){
  int n = V1.GetSize();
  Vector V(n);
  for (int i = 0 ; i < n ; i++){V[i] = C.Read(i) * V1.Read(i);}
  
  double S = 0;
  for (int i = 0 ; i < n ; i++){
    S += V.Read(i) * V2.Read(i);
  }
  return S;
}

Vector GC(const CSRMatrix& A,
	  const Vector& b,
	  const int nP){

  //PARAMETRES DU CALCUL
  int nI = 10000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0.00000000000);

  //MONITORING
  ofstream log;
  string n_str;
  stringstream convert;
  convert << nP;
  n_str = convert.str();
  string name = "TIME.txt";
  log.open(name.c_str());
  double t0=omp_get_wtime(), t1=0, t2=0, t3=0;

  //INITIALISATION
  Vector g = parMult(A, X, nP) - b;
  Vector h = -g;  
  Vector Ah(nR), g_new(nR);
  double rau=0, gamma=0, r=0;

  //ITERATIONS
  for (int i = 0 ; i < nI ; i++){
    for (int j = 0 ; j < nR ; j++){if (j%(nR/4) == 0){cout << X[j] << " "; }}
    cout << endl;
    t2=omp_get_wtime();
    
    Ah = parMult(A, h, nP);
    rau = -scal(g, h)/scal(h, Ah);
    X += h * rau;
    g_new = g + Ah * rau;
    gamma = scal(g_new, g_new)/scal(g, g);
    g = g_new;
    h = -g + h * gamma;
    r = scal(g_new, g_new);
    if( r < eps ){return X;}
    
    t3 = omp_get_wtime();
    log << "Itération\t" << i << ": R =\t" << r << ", t =\t" << t3 - t2 << " s" << endl; 
  }

  //MONITORING
  t1 = omp_get_wtime() - t0;
  log << "Temps d'execution total =\t" << t1 << " s" << endl;
  log.close();
  
  return X;
}

//Gradient conjugué préconditionné par C
Vector GCP(const CSRMatrix& A,
	   const Vector& b,
	   const Matrix& C,
	   const int nP){

  //PARAMETRES DU CALCUL
  int nI = 100000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0.00000000000);

  //MONITORING
  ofstream log;
  string n_str;
  stringstream convert;
  convert << nP;
  n_str = convert.str();
  string name = "TIME.txt";
  log.open(name.c_str());
  double t0=omp_get_wtime(), t1=0, t2=0, t3=0;

  //INITIALISATION
  Vector G = parMult(A, X, nP) - b;
  Vector H = -C*G;  
  Vector AH(nR), G_new(nR);
  double rau=0, gamma=0, r=0;
  
  //ITERATIONS
  for (int i = 0 ; i < nI ; i++){
    for (int j = 0 ; j < nR ; j++){if (j%(nR/5) == 0){cout << X[j] << " "; }}
    cout << endl;
    t2=omp_get_wtime();
    
    AH = parMult(A, H, nP);
    rau = -scal(G, H)/scal(H, AH);
    X += H * rau;
    G_new = G + AH * rau;
    gamma = scalC(G_new, G_new, C) / scalC(G, G, C);
    G = G_new;
    H = -C*G + H * gamma;
    r = scalC(G, G, C);
    if( r < eps ){return X;}
      
    t3 = omp_get_wtime();
    log << "Itération\t" << i << ": R =\t" << r << ", t =\t" << t3 - t2 << " s" << endl; 
  }
}

Vector GCPDiag(const CSRMatrix& A,
	   const Vector& b,
	   const Vector& C,
	   const int nP){

  //PARAMETRES DU CALCUL
  int nI = 2000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0);

  //MONITORING
  ofstream log;
  string n_str;
  stringstream convert;
  convert << nP;
  n_str = convert.str();
  string name = "TIME.txt";
  log.open(name.c_str());
  double t0=omp_get_wtime(), t1=0, t2=0, t3=0;

  //INITIALISATION
  Vector G = parMult(A, X, nP) - b;
  Vector H(nR);
  for (int i = 0 ; i < nR ; i++){ H[i] = - C.Read(i) * G.Read(i);}
  Vector AH(nR), G_new(nR);
  double rau=0, gamma=0, r=0;
  
  //ITERATIONS
  for (int i = 0 ; i < nI ; i++){
    for (int j = 0 ; j < nR ; j++){if (j%(nR/5) == 0){cout << X[j] << " "; }}
    cout << endl;
    t2=omp_get_wtime();
    
    AH = parMult(A, H, nP);
    rau = -scal(G, H)/scal(H, AH);
    X += H * rau;
    G_new = G + AH * rau;
    gamma = scalC(G_new, G_new, C) / scalC(G, G, C);
    G = G_new;
    Vector cg(nR);
    for (int k = 0 ; k < nR ; k++){ cg[k] = C.Read(k) * G.Read(k);}
    H = -cg + H * gamma;
    r = scalC(G, G, C);
    if( r < eps ){return X;}
      
    t3 = omp_get_wtime();
    log << "Itér " << i << ":    \tR= " << r << ",        \tt = " << t3 - t2 << ",    \t|g| = " << G.norm() << endl; 
  }
}
