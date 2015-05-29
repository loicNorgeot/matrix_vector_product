#ifndef GCHEADERDEF
#define GCHEADERDEF

#include "vector.h"
#include "csrmatrix.h"
#include "matrix.h"

//PROTOTYPES
Vector GC(const CSRMatrix& A, const Vector& b, const int nP);
Vector GCP(const CSRMatrix& A, const Vector& b, const Matrix& C, const int nP);
double scal(const Vector& V1, const Vector& V2);
double scalC(const Vector& V1, const Vector& V2, const Matrix& C);

#endif
