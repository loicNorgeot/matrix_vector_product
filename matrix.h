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
  Matrix(const Vector& V);
  Matrix(int size);
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

  //Appending, size modifications
  void resize(int newRows, int newCols);
  void delRow(int nRowsToDel=1);
  void addRow(int nRowsToAdd=1);
  void addCol(int nColsToAdd=1);
  void delCol(int nColsToDel=1);
  void addRow(const Vector& newRow);
  void addCol(const Vector& newCol);

  //Extracting
  Vector row(int row);
  Vector col(int col);

  //Friend functions
  //Identity
  friend Matrix ID(int size, double val);
  friend Matrix ID(const Vector& V);
  //Vector multiplication
  friend Vector operator*(const Matrix& m, const Vector& v);
  friend Vector operator *(const Vector& v, const Matrix& m);
  
  //CSRMatrix class as a friend
  friend class CSRMatrix;

};

//Friend functions
Matrix ID(int size, double val);
Matrix ID(const Vector& V);
Vector operator*(const Matrix& m, const Vector& v);
Vector operator*(const Vector& v, const Matrix& m);

#endif

