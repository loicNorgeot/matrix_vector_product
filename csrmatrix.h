//Header de la classe CSRMatrix
#ifndef CSRMATRIXHEADERDEF
#define CSRMATRIXHEADERDEF

#include "vector.h"

class CSRMatrix{
private:
  double *mA;
  unsigned int *mIA;
  int *mJA;
  unsigned int mNNZ;
  int mNumRows;
public:
  CSRMatrix(std::string inPath, std::string name, const int nbProcs);
  ~CSRMatrix();

  unsigned int GetNNZ() const;
  int GetNumberOfRows() const;
  Vector toDiagonal() const;

  friend Vector operator*(const CSRMatrix& m, const Vector& v);
  friend Vector parMult(const CSRMatrix& m, const Vector& v, const int nbProcs);
};

Vector operator*(const CSRMatrix& m, const Vector& v);
Vector parMult(const CSRMatrix& m, const Vector& v, const int nbProcs);

#endif
