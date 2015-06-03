#include <string>
#include <iostream>
#include <string>
#include <cstdlib>

#include "omp.h"
#include "binaryIO.h"
#include "chrono.h"

using namespace std;

int main(int argc, char* argv[]){

  ///////////////////////////////////////////
  //Variables

  const int nP = atoi(argv[1]);
  string inPath = "/work/norgeot/";
  string varName = "SIZE";
  string name(getenv(varName.c_str()));
  double t = omp_get_wtime();
  int inc = 0;


  ///////////////////////////////////////////
  //READING

  //Variables
  int nR = 0;
  unsigned int nnz = 0;
  getHeaderInfo(nR, nnz, inPath, name);
  t = chrono(t, inc, "Header reading");

  //Initialization
  unsigned int *IAt = new unsigned int[nR + 1];
  int *JAt = new int[nnz];
  double *At = new double[nnz];
  double *V = new double[nR];
  t = chrono(t, inc, "Temporary arrays declaration");

  //Reading
  brMatrix(IAt, JAt, At, nR, nnz, inPath, name);
  brVector(V, nR, inPath, name);
  t = chrono(t, inc, "Temporary arrays reading");
  const int chunk = nR/nP;


  ///////////////////////////////////////////
  //FIRST TOUCH INITIALIZATION

  //Variables declaration
  unsigned int *IA = new unsigned int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  t = chrono(t, inc, "Final arrays declaration");

  //Parallel Initialization
#pragma omp parallel for num_threads(nP) schedule(static, chunk)
  for (int i = 0 ; i < nR; i++ ){
    IA[i] = IAt[i];
    IA[i+1] = IAt[i+1];
    for(unsigned int j=IA[i]; j<IA[i+1]; j++){
      A[j] = At[j];
      JA[j] = JAt[j];
    }
  }
  t = chrono(t, inc, "First touch Initialization");

  //Unallocation
  delete[] IAt;
  delete[] JAt;
  delete[] At;
  t = chrono(t, inc, "Temporary arrays unallocation");


  /////////////////////////////////////////////////////:
  //PRODUCT COMPUTATION

  //Result declaration
  double *sol = new double[nR];

  //Local copy
  double *v_copy=NULL;
  v_copy = new double[nR];
#pragma omp for schedule(static, chunk)
  for(int i=0; i<nR; i++){
    v_copy[i] = V[i];
  }
  t = chrono(t, inc, "Vector local copy");

  //Actual computation
#pragma omp parallel for num_threads(nP) schedule(static, chunk) firstprivate(v_copy)
  for(int i=0; i<nR; i++){
    double s_temp = 0.0;
    for(unsigned int j=IA[i]; j<IA[i+1]; j++){
      s_temp += A[j]*v_copy[JA[j]];
    }
    sol[i] = s_temp;
  }
  t = chrono(t, inc, "Parallel product");
  delete[] v_copy;


  ////////////////////////////////////////////////////////////::
  //CLEANING

  delete[] V;
  delete[] A;
  delete[] IA;
  delete[] JA;
  t = chrono(t, inc, "Final unallocation");


  ////////////////////////////////////////////////////////////::
  //PRINTING

  cout << "SOL = " << sol[0] << ", " << sol[1] << " ... " << sol[nR-2] << ", " << sol[nR-1] << endl;
  delete[] sol;

}
