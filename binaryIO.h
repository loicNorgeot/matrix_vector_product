#ifndef BINARYIOHEADERDEF
#define BINARYIOHEADERDEF

#include <string>
using namespace std;

//Ecriture
FILE * bwopen(string fileName);
void bwArray(int *X, int lX, string fileName);
void bwArray(double *X, int lX, string fileName);
void bwArray(unsigned int *X, int lX, string fileName);
//Wrappers
void bwMatrix(int *IA, int *JA, double *A, int nR, int nnz, string commonName);
void bwVector(double *V, int nR, string commonName);

//Lecture
FILE * bropen(string fileName);
void brArray(int *& X, int lX, string fileName);
void brArray(double *& X, int lX, string fileName);
void brArray(unsigned int *& X, int lX, string fileName);
//Wrappers
void getHeaderInfo(int& nR, int& nnz, string commonName);
void brVector(double *& V, int nR, string commonName);
void brMatrix(int *& IA, int *& JA, double *& A, int nR, int nnz, string commonName);

//Tests
void wTest();
void rTest();

#endif
