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

template<typename T> std::vector<T> split(const std::string& line) {
  std::istringstream is(line);
  return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
}

int main(){

  //Variables globales
  int nR;
  int nnz;
  int S;
  const int nP = 32;

  // ----------------------------------------
  //                LECTURE
  // ----------------------------------------
  //Définition des variables de lecture
  FILE *aFile, *iaFile, *jaFile;
  string dataPath = "/work/norgeot/";
  string SIZE = "1e+09"
  string mRoot = dataPath + "matrix_" + SIZE;
  string vRoot = dataPath + "vector_" + SIZE;

  //MATRICE
  //Lecture du header
  ifstream infile((mRoot + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    nR = line[0];
    nnz = line[1];
    cout << nnz << " " << nR << endl;
  }
  //Définition du chunk
  const int chunk = nR/nP;
  //Lecture de IA
  int *IAt=NULL;
  IAt = new int[nR+1];
  iaFile=fopen((mRoot + "_IA.bin").c_str(),"rb");
  if (!iaFile){
    printf("Unable to open file!");
  }
  fread(IAt,sizeof(*IAt), nR + 1,iaFile);
  fclose(iaFile);
  //Lecture de JA
  int *JAt=NULL;
  JAt = new int[nnz];
  jaFile=fopen((mRoot + "_JA.bin").c_str(),"rb");
  if (!jaFile){
    printf("Unable to open file!");
  }
  fread(mJAt,sizeof(*JAt),nnz,jaFile);
  fclose(jaFile);
  //Lecture de A
  double *At=NULL;
  At = new double[nnz];
  aFile=fopen((mRoot + "_A.bin").c_str(),"rb");
  if (!aFile){
    printf("Unable to open file!");
  }
  fread(At,sizeof(*At),nnz,aFile);
  fclose(aFile);

  //VECTEUR
  //Lecture du header
  std::string str;
  std::ifstream infile((vRoot + "_H.data").c_str());
  while(std::getline(infile, str)){
    std::vector<int> line = split<int>(str);
    S = line[0];
  }
  //Vérification de la taille
  assert(S==nR);
  //Enregistrement des valeurs
  FILE *vFile;
  double *V = new double[S];
  vFile=fopen((vRoot + "_V.bin").c_str(),"rb");
  if (!vFile){
    printf("Unable to open file!");
  }
  fread(V,sizeof(*V),S,vFile);
  fclose(vFile);

  //Nettoyage des FILES
  delete vFile;
  delete aFile;
  delete iaFile;
  delete jaFile;

  // ----------------------------------------
  //              INITIALISATION
  // ----------------------------------------
  //Initialisation des pointeurs
  int *IA = new int[nR + 1];
  int *JA = new int[nnz];
  int *A = new double[nnz];

  //Initialisation en parallèle
#pragma omp parallel for num_threads(nP) schedule(static, chunk)
  for (int i = 0 ; i < nR; i++ ){
    IA[i] = IAt[i];
    IA[i+1] = IAt[i+1];
    for(int j=IA[i]; j<IA[i+1]; j++){
      A[j] = At[j];
      JA[j] = JAt[j];
    }
  }

  //Desallocation
  delete[] IAt;
  delete[] JAt;
  delete[] At;

  // ----------------------------------------
  //             MULTIPLICATION
  // ----------------------------------------
  //Initialisation du résultat
  Vector sol(nR);

  //Zone parallèle
#pragma omp parallel num_threads(nP)
  {
    //Copie locale du vecteur
    double *v_copy=NULL;
    v_copy = new double[nR];
#pragma omp for schedule(static, chunk)
    for(int i=0; i<nR; i++){
      v_copy[i] = V[i];
    }
    //Produit parallèle
#pragma omp for schedule(static, chunk)
    for(int i=0; i<nR; i++){
      double s_temp = 0.0;
      for(int j=IA[i]; j<IA[i+1]; j++){
	s_temp += A[j]*v_copy[JA[j]];
      }
      sol[i] = s_temp;
    }
    //Desallocation
    delete[] v_copy;
  }

  // ----------------------------------------
  //             NETTOYAGE
  // ----------------------------------------
  delete[] V;
  delete[] A;
  delete[] IA;
  delete[] JA;
}
