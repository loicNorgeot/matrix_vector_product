#ifndef BINARYIOHEADERDEF
#define BINARYIOHEADERDEF

#include <string>
using namespace std;

//Ecriture
FILE * bwopen(string fileName);
//Surcharge
void bwArray(int *X, int lX, string fileName);
void bwArray(double *X, int lX, string fileName);
void bwArray(unsigned int *X, int lX, string fileName);
void bwArray(int *X, unsigned int lX, string fileName);
void bwArray(double *X, unsigned int lX, string fileName);
void bwArray(unsigned int *X, unsigned int lX, string fileName);
//Wrappers
void bwMatrix(unsigned int *IA, int *JA, double *A, int nR, unsigned int nnz, string outPath, string name);
void bwVector(double *V, int nR, string outPath, string name);

//Lecture
FILE * bropen(string fileName);
//Surcharge
void brArray(int *& X, int lX, string fileName);
void brArray(double *& X, int lX, string fileName);
void brArray(unsigned int *& X, int lX, string fileName);
void brArray(int *& X, unsigned int lX, string fileName);
void brArray(double *& X, unsigned int lX, string fileName);
void brArray(unsigned int *& X, unsigned int lX, string fileName);
//Wrappers
void getHeaderInfo(int& nR, unsigned int& nnz, string inPath, string name);
void brVector(double *& V, int nR, string inPath, string name);
void brMatrix(unsigned int *& IA, int *& JA, double *& A, int nR, unsigned int nnz, string inPath, string name);

//Tests
void wExample(string outPath, string name);
void rExample(string inPath, string name);

#endif
