//Header de la classe matrix
#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF

#include "csrmatrix.h"
#include "vector.h"

class Matrix{

private:

  double** mData;
  int mNumRows;
  int mNumCols;

public:

  //Constructors and destructors
  Matrix(const Matrix& otherMatrix);
  Matrix(int rows, int cols);
  ~Matrix();

  //Private variables accessing
  int GetNumberOfRows() const;
  int GetNumberOfCols() const;
  double& operator()(int i, int j);

  //Operator Overloading
  Matrix& operator=(const Matrix& otherMatrix);
  Matrix operator+() const;
  Matrix operator+(const Matrix& m) const;
  Matrix operator-() const;
  Matrix operator-(const Matrix& m) const;
  Matrix operator*(double a) const;
  Matrix operator*(const Matrix& m) const;

  //Friend functions
  friend Vector operator*(const Matrix& m, const Vector& v);
  friend Vector operator *(const Vector& v, const Matrix& m);
  
  //Classe CSRmatrix définie comme amie pour avoir accès
  friend class CSRMatrix;

};

//Friend functions
Vector operator*(const Matrix& m, const Vector& v);
Vector operator*(const Vector& v, const Matrix& m);

#endif

