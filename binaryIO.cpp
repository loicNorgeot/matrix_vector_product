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


///////////////////////////////////////////
//WRITING

//Binary file opening
FILE * bwopen(string fileName){
  FILE *file;
  file = fopen(fileName.c_str(),"wb");
  assert(file);
  return file;
}

//Overloading the writing methods
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
void bwArray(int *X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}
void bwArray(double *X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}
void bwArray(unsigned int *X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bwopen(fileName);
  fwrite(X,sizeof(*X), lX, file);
  fclose(file);
}

//Writing CSR elements in binary files
void bwMatrix(unsigned int *IA,
	      int *JA,
	      double *A,
	      int nR,
	      unsigned int nnz,
	      string outPath,
	      string name){
  //Header
  ofstream out;
  string root = outPath + "matrix_" + name;
  out.open((root + "_H.data").c_str());
  out << nR << " " << nnz;
  out.close();
  //Arrays
  bwArray(IA, nR + 1, root + "_IA.bin");
  bwArray(JA, nnz, root + "_JA.bin");
  bwArray(A, nnz, root + "_A.bin");
}

//Writing a Vector in binary files
void bwVector(double *V,
	      int nR,
	      string outPath,
	      string name){
  //Header
  ofstream out;
  string root = outPath + "vector_" + name;
  out.open((root + "_H.data").c_str());
  out << nR;
  out.close();
  //Array
  bwArray(V, nR, root + "_V.bin");
}


///////////////////////////////////////////
//READING

//Read opening of a binary file
FILE * bropen(string fileName){
  FILE *file;
  file = fopen(fileName.c_str(),"rb");
  assert(file);
  return file;
}

//Overloading the reading methods
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
void brArray(int *& X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}
void brArray(double *& X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}
void brArray(unsigned int *& X,
	     unsigned int lX,
	     string fileName){
  FILE *file = bropen(fileName);
  fread(X, sizeof(*X), lX, file);
  fclose(file);
}

//Gathering of header informations (nR and nnz)
void getHeaderInfo(int& nR,
		   unsigned int& nnz,
		   string inPath,
		   string name){
  int nRV = 0, nRM = 0;
  //Vector Header
  ifstream infile((inPath + "vector_" + name + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    nRV = line[0];
  }
  //Matrix header
  ifstream infileB((inPath + "matrix_" + name + "_H.data").c_str());
  string strB;
  while(getline(infileB, str)){
    vector<int> line = split<int>(str);
    nRM = line[0];
    nnz = (unsigned int)line[1];
  }
  //Comparing
  assert(nRM==nRV);
  nR = nRM;
}

//Reading a vector from binary files
void brVector(double *& V,
	      int nR,
	      string inPath,
	      string name){
  V = new double[nR];
  brArray(V, nR, inPath + "vector_" + name + "_V.bin");
}

//Reading a CSRMatrix from binary files
void brMatrix(unsigned int *& IA,
	      int *& JA,
	      double *& A,
	      int nR,
	      unsigned int nnz,
	      string inPath,
	      string name){
  brArray(IA, nR + 1, inPath + "matrix_" + name + "_IA.bin");
  brArray(JA, nnz, inPath + "matrix_" + name + "_JA.bin");
  brArray(A, nnz, inPath + "matrix_" + name + "_A.bin");
}


//////////////////////////////////////////////////////////
//TESTING AND EXAMPLES

//Writing example
void writingExample(string outPath,
		    string name){
  //Variables
  const int nR = 10;
  const unsigned int nnz = 20;
  //Initialization
  unsigned int *IA = new unsigned int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  double *V = new double[nR];
  for(int i = 0 ; i < nR ; i++){
    V[i] = 0.1 * i;
  }
  for(int i = 0 ; i < nR+1 ; i++){
    IA[i] = -i;
  }
  for(unsigned int i = 0 ; i < nnz ; i++){
    JA[i] = i;
    A[i] = i * 0.5;
  }
  //Writing
  bwMatrix(IA, JA, A, nR, nnz, outPath, name);
  bwVector(V, nR, outPath, name);
  //Print
  cout << "Writing test:" << endl;
  cout << "nR = " << nR << endl;
  cout << "nnz = " << nnz << endl;
  cout << "End of writing\n" << endl;
}

//Reading example
void readingExample(string inPath,
		    string name){
  //Variables
  int nR=0;
  unsigned int nnz=0;
  getHeaderInfo(nR, nnz, inPath, name);
  //Initialization
  unsigned int *IA = new unsigned int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  double *V = new double[nR];
  //Reading
  brMatrix(IA, JA, A, nR, nnz, inPath, name);
  brVector(V, nR, inPath, name);
  //Print
  cout << "Reading test:" << endl;
  cout << "nR = " << nR << endl;
  cout << "nnz = " << nnz << endl;
  cout << "IA = " << IA[0] << " ... " << IA[nR-1] << endl;
  cout << "JA = " << JA[0] << " ... " << JA[nnz-1] << endl;
  cout << "A = " << A[0] << " ... " << A[nnz-1] << endl;
  cout << "V = " << V[0] << " ... " << V[nR-1] << endl;
}
