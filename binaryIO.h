#ifndef BINARYIOHEADERDEF
#define BINARYIOHEADERDEF

#include <string>

using namespace std;


//Writing
FILE * bwopen(string fileName);
void bwArray(int *X, int lX, string fileName);
void bwArray(double *X, int lX, string fileName);
void bwArray(unsigned int *X, int lX, string fileName);
void bwArray(int *X, unsigned int lX, string fileName);
void bwArray(double *X, unsigned int lX, string fileName);
void bwArray(unsigned int *X, unsigned int lX, string fileName);
void bwMatrix(unsigned int *IA, int *JA, double *A, int nR, unsigned int nnz, string outPath, string name);
void bwVector(double *V, int nR, string outPath, string name);

//Reading
FILE * bropen(string fileName);
void brArray(int *& X, int lX, string fileName);
void brArray(double *& X, int lX, string fileName);
void brArray(unsigned int *& X, int lX, string fileName);
void brArray(int *& X, unsigned int lX, string fileName);
void brArray(double *& X, unsigned int lX, string fileName);
void brArray(unsigned int *& X, unsigned int lX, string fileName);
void getHeaderInfo(int& nR, unsigned int& nnz, string inPath, string name);
void brVector(double *& V, int nR, string inPath, string name);
void brMatrix(unsigned int *& IA, int *& JA, double *& A, int nR, unsigned int nnz, string inPath, string name);

//Examples
void writingExample(string outPath, string name);
void readingExample(string inPath, string name);

#endif
