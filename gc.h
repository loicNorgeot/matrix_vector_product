#ifndef GCHEADERDEF
#define GCHEADERDEF

#include "vector.h"
#include "csrmatrix.h"
#include "matrix.h"

//Scalar Products
double scalC(const Vector& v1, const Vector& v2, const Vector& C);
double scalC(const Vector& v1, const Vector& v2, const Matrix& C);
double scal(const Vector& v1, const Vector& v2);

//Conjugate Gradient
Vector GC(const CSRMatrix& A, const Vector& b, const int nP);
Vector PGC(const CSRMatrix& A, const Vector& b, const Vector& C, const int nP);
Vector PGC(const CSRMatrix& A, const Vector& b, const Matrix& C, const int nP);

#endif
