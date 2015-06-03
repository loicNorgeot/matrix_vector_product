#include <string>
#include <iostream>
#include <string>
#include <cstdlib>

#include "csrmatrix.h"
#include "matrix.h"
#include "vector.h"
#include "chrono.h"
#include "gc.h"

#include "omp.h"

using namespace std;


///////////////////////////////////////////
//SCALAR PRODUCTS

//With Preconditionner, Vector Object
double scalC(const Vector& v1,
	    const Vector& v2,
	    const Vector& C){
  return scalProduct(vectProduct(C,v1), v2);
}

//With Preconditionner, Matrix Object
double scalC(const Vector& v1,
	    const Vector& v2,
	    const Matrix& C){
  return scalProduct(C*v1, v2);
}

//Without Preconditionner
double scal(const Vector& v1,
	    const Vector& v2){
  return scalProduct(v1,v2);
}


///////////////////////////////////////////
//CONJUGATE GRADIENT

//Classical, no preconditionning
Vector GC(const CSRMatrix& A,
	  const Vector& b,
	  const int nP){
  //Parameters
  int nI = 10000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0.00000000000);
  //Initialization
  Vector g = A.parallelProduct(X, nP) - b;
  Vector h = -g;  
  Vector Ah(nR), g_new(nR);
  double rau=0, gamma=0, r=0;
  //Iterations
  for (int i = 0 ; i < nI ; i++){    
    Ah = A.parallelProduct(h, nP);
    rau = -scal(g, h)/scal(h, Ah);
    X += h * rau;
    g_new = g + Ah * rau;
    gamma = scal(g_new, g_new)/scal(g, g);
    g = g_new;
    h = -g + h * gamma;
    r = scal(g_new, g_new);
    if( r < eps ){return X;}
  } 
  return X;
}

//Preconditionning with a diagonal matrix, Vector object
Vector PGC(const CSRMatrix& A,
	   const Vector& b,
	   const Vector& C,
	   const int nP){
  //Parameters
  int nI = 2000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0);
  //Initialization
  double t = omp_get_wtime();
  int inc = 0;
  Vector G = A.parallelProduct(X, nP) - b;
  t = chrono(t, inc, "G initialization");
  Vector CG = vectProduct(C, G);
  Vector H = - CG;
  t = chrono(t, inc, "H initialization");
  Vector AH(nR), G_new(nR);
  double rau=0, gamma=0, r=0;
  //Iterations
  for (int i = 0 ; i < nI ; i++){
    if(i==0){t = chrono(t, inc, "Iterations start");}   
    AH = A.parallelProduct(H, nP);
    if(i==0){t = chrono(t, inc, "A * H product");}
    rau = -scal(G, H)/scal(H, AH);
    if(i==0){t = chrono(t, inc, "Rau computation");}
    X += H * rau;
    if(i==0){t = chrono(t, inc, "X update");}
    G_new = G + AH * rau;
    if(i==0){t = chrono(t, inc, "G_new computation");}
    gamma = scalC(G_new, G_new, C) / scalC(G, G, C);
    if(i==0){t = chrono(t, inc, "Gamma computation");}
    G = G_new;
    CG = vectProduct(C,G);
    if(i==0){t = chrono(t, inc, "CG update");}
    H = -CG + H * gamma;
    if(i==0){t = chrono(t, inc, "H update");}
    r = scalC(G, G, C);
    if(i==0){t = chrono(t, inc, "R computation");}
    if( r < eps ){return X;}
  }
  return X;
}

//Preconditionning with a matrix, Matrix object
Vector PGC(const CSRMatrix& A,
	   const Vector& b,
	   const Matrix& C,
	   const int nP){
  //Parameters
  int nI = 2000;
  double eps = 1e-5;
  int nR = A.GetNumberOfRows();
  Vector X(nR, 0);
  //Initialization
  double t = omp_get_wtime();
  int inc = 0;
  Vector G = A.parallelProduct(X, nP) - b;
  t = chrono(t, inc, "G initialization");
  Vector CG = C * G;
  Vector H = - CG;
  t = chrono(t, inc, "H initialization");
  Vector AH(nR), G_new(nR);
  double rau=0, gamma=0, r=0;
  //Iterations
  for (int i = 0 ; i < nI ; i++){
    if(i==0){t = chrono(t, inc, "Iterations start");}   
    AH = A.parallelProduct(H, nP);
    if(i==0){t = chrono(t, inc, "A * H product");}
    rau = -scal(G, H)/scal(H, AH);
    if(i==0){t = chrono(t, inc, "Rau computation");}
    X += H * rau;
    if(i==0){t = chrono(t, inc, "X update");}
    G_new = G + AH * rau;
    if(i==0){t = chrono(t, inc, "G_new computation");}
    gamma = scalC(G_new, G_new, C) / scalC(G, G, C);
    if(i==0){t = chrono(t, inc, "Gamma computation");}
    G = G_new;
    CG = C * G;
    if(i==0){t = chrono(t, inc, "CG update");}
    H = -CG + H * gamma;
    if(i==0){t = chrono(t, inc, "H update");}
    r = scalC(G, G, C);
    if(i==0){t = chrono(t, inc, "R computation");}
    if( r < eps ){return X;}
  }
  return X;
}
