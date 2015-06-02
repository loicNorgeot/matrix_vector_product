#include <fstream>
#include <string>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <sstream>
#include <iterator>
#include <vector>

#include "binaryIO.h"

using namespace std;

template<typename T>
vector<T> split(const string& line) {
  istringstream is(line);
  return vector<T>(istream_iterator<T>(is), istream_iterator<T>());
}



//####################
//      ECRITURE     #
//####################

//Ouverture d'un fichier binaire
FILE * bwopen(string fileName){
  FILE *file;
  file = fopen(fileName.c_str(),"wb");
  assert(file);
  return file;
}

//Surcharge de l'écriture d'un tableau dans un fichier
void bwArray(int *X,
	     int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}
void bwArray(double *X,
	     int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}
void bwArray(unsigned int *X,
	     int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}

//Ecriture d'une matrice CSR en format binaire
void bwMatrix(int *IA,
	      int *JA,
	      double *A,
	      int nR,
	      int nnz,
	      string commonName){
  //Ecriture du header
  ofstream out;
  out.open(("matrix_" + commonName + "_H.data").c_str());
  out << nR << " " << nnz;
  out.close();
  //Ecriture des tableaux
  bwArray(IA, nR + 1, "matrix_" + commonName + "_IA.bin");
  bwArray(JA, nnz, "matrix_" + commonName + "_JA.bin");
  bwArray(A, nnz, "matrix_" + commonName + "_A.bin");
}

//Ecriture d'un vecteur en format binaire
void bwVector(double *V,
	      int nR,
	      string commonName){
  //Ecriture du header
  ofstream out;
  out.open(("vector_" + commonName + "_H.data").c_str());
  out << nR;
  out.close();
  //Ecriture du vecteur
  bwArray(V, nR, "vector_" + commonName + "_V.bin");
}



//###################
//      LECTURE     #
//###################

//Ouverture d'un fichier binaire en lecture
FILE * bropen(string fileName){
  FILE *file;
  file = fopen(fileName.c_str(),"rb");
  assert(file);
  return file;
}

//Surcharge de la lecture d'un tableau en binaire
void brArray(int *& X,
	     int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}
void brArray(double *& X,
	     int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}
void brArray(unsigned *& X,
	     int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}

void getHeaderInfo(int& nR,
		   int& nnz,
		   string commonName){
  int nRV = 0, nRM = 0;
  //Header du vecteur
  ifstream infile(("vector_" + commonName + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    nRV = line[0];
  }
  //Header de la matrice
  ifstream infileB(("matrix_" + commonName + "_H.data").c_str());
  string strB;
  while(getline(infileB, str)){
    vector<int> line = split<int>(str);
    nRM = line[0];
    nnz = line[1];
  }
  //Comparaison
  assert(nRM==nRV);
  nR = nRM;
}

void brVector(double *& V,
	      int nR,
	      string commonName){
  V = new double[nR];
  brArray(V, nR, "vector_" + commonName + "_V.bin");
}

void brMatrix(int *& IA,
	      int *& JA,
	      double *& A,
	      int nR,
	      int nnz,
	      string commonName){
  brArray(IA, nR + 1, "matrix_" + commonName + "_IA.bin");
  brArray(JA, nnz, "matrix_" + commonName + "_JA.bin");
  brArray(A, nnz, "matrix_" + commonName + "_A.bin");
}



//###############
//     TESTS    #
//###############

void wTest(){
  //Variables
  const int nR = 10;
  const int nnz = 20;
  
  //Initialisation
  int *IA = new int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  double *V = new double[nR];
  for(int i = 0 ; i < nR ; i++){
    V[i] = 0.1 * i;
  }
  for(int i = 0 ; i < nR+1 ; i++){
    IA[i] = -i;
  }
  for(int i = 0 ; i < nnz ; i++){
    JA[i] = i;
    A[i] = i * 0.5;
  }

  //Ecriture
  bwMatrix(IA, JA, A, nR, nnz, "test");
  bwVector(V, nR, "test");

  //Print
  cout << "Tests d'écriture:" << endl;
  cout << "nR = " << nR << endl;
  cout << "nnz = " << nnz << endl;
  cout << "Ecriture terminée\n" << endl;
}


void rTest(){
  //Variables
  int nR=0, nnz=0;
  getHeaderInfo(nR, nnz, "test");

  //Initialisation
  int *IA = new int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  double *V = new double[nR];

  //Lecture
  brMatrix(IA, JA, A, nR, nnz, "test");
  brVector(V, nR, "test");

  //Print
  cout << "Tests d'écriture:" << endl;
  cout << "nR = " << nR << endl;
  cout << "nnz = " << nnz << endl;
  cout << "IA = " << IA[0] << " ... " << IA[nR-1] << endl;
  cout << "JA = " << JA[0] << " ... " << JA[nnz-1] << endl;
  cout << "A = " << A[0] << " ... " << A[nnz-1] << endl;
  cout << "V = " << V[0] << " ... " << V[nR-1] << endl;
}
