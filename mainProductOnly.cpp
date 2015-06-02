//Prend en argument le nombre de processeurs souhaités
#include "omp.h"
#include "binaryIO.h"

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

template<typename T> std::vector<T> split(const std::string& line) {
  std::istringstream is(line);
  return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}


int main(int argc, char* argv[]){

  const int nP = atoi(argv[1]);
  string inPath = "/work/norgeot/";
  string name = "pascal";

  // ----------------------------------------
  //                LECTURE
  // ----------------------------------------
  //Déclaration et variables
  int nR = 0;
  unsigned int nnz = 0;
  getHeaderInfo(nR, nnz, inPath, name);

  //Initialisation
  unsigned int *IAt = new unsigned int[nR + 1];
  int *JAt = new int[nnz];
  double *At = new double[nnz];
  double *V = new double[nR];

  //Lecture
  brMatrix(IAt, JAt, At, nR, nnz, inPath, name);
  brVector(V, nR, inPath, name);
 
  //Définition du chunk
  const int chunk = nR/nP;

  // ----------------------------------------
  //              FIRST TOUCH
  // ----------------------------------------
  //Initialisation
  unsigned int *IA = new unsigned int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];

  //Initialisation en parallèle
#pragma omp parallel for num_threads(nP) schedule(static, chunk)
  for (int i = 0 ; i < nR; i++ ){
    IA[i] = IAt[i];
    IA[i+1] = IAt[i+1];
    for(unsigned int j=IA[i]; j<IA[i+1]; j++){
      A[j] = At[j];
      JA[j] = JAt[j];
    }
  }

  //Desallocation
  delete[] IAt;
  delete[] JAt;
  delete[] At;

  // ----------------------------------------
  //             PRODUIT
  // ----------------------------------------
  //Initialisation du résultat
  double *sol = new double[nR];

  //Copie locale du vecteur
  double *v_copy=NULL;
  v_copy = new double[nR];
#pragma omp for schedule(static, chunk)
  for(int i=0; i<nR; i++){
    v_copy[i] = V[i];
  }

  //Zone parallèle
#pragma omp parallel for num_threads(nP) schedule(static, chunk) firstprivate(v_copy)
  for(int i=0; i<nR; i++){
    double s_temp = 0.0;
    for(int j=IA[i]; j<IA[i+1]; j++){
      s_temp += A[j]*v_copy[JA[j]];
    }
    sol[i] = s_temp;
  }
  
  // ----------------------------------------
  //             NETTOYAGE
  // ----------------------------------------
  delete[] v_copy;
  delete[] V;
  delete[] A;
  delete[] IA;
  delete[] JA;
}
