#ifndef FONCTIONS_MAINHEADERDEF
#define FONCTIONS_MAINHEADERDEF

#include "vector.h"
#include "matrix.h"
#include "linearsystem.h"
#include "csrmatrix.h"

Matrix createBigMatrix(int size, double ratio);
void testVectorClass();
void testMatrixClass();
void testLinearSystem();
void testMatrixVectorProduct();
void launchParallelCSRProductOnRandomMatrix(int size, double ratio, int numProcs);
void launchParallelCSRProductOnData(int nP);
Vector getResult();

#endif
