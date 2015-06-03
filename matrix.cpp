#include <cmath>
#include <iostream>
#include <cassert>
#include <string>

#include "matrix.h"
#include "vector.h"

using namespace std;


///////////////////////////////////////////
//CONSTRUCTORS & DESTRUCTOR

//Copy
Matrix::Matrix(const Matrix& otherMatrix){
  mNumRows = otherMatrix.GetNumberOfRows();
  mNumCols = otherMatrix.GetNumberOfCols();
  mData = new double*[mNumRows];
  for(int i=0; i<mNumRows; i++){
    mData[i] = new double [mNumCols];
  }
  for(int i=0; i<mNumRows; i++){
    for(int j=0; j<mNumCols; j++){
      mData[i][j] = otherMatrix.mData[i][j];
    }
  }
}

//Size
Matrix::Matrix(int rows, int cols){
  assert(rows>0);
  assert(cols>0);
  mNumRows = rows;
  mNumCols = cols;
  mData = new double* [mNumRows];
  for(int i=0; i<mNumRows; i++){
    mData[i] = new double [mNumCols];
  }
  for(int i=0; i<mNumRows; i++){
    for(int j=0; j<mNumCols; j++){
      mData[i][j] = 0.0;
    }
  }
}

//Destructor
Matrix::~Matrix(){
  for(int i=0; i<mNumRows; i++){
    delete[] mData[i];
  }
  delete[] mData;
}


///////////////////////////////////////////////////
//READING PRIVATE VARIABLES

//Number of Rows
int Matrix::GetNumberOfRows() const{
  return mNumRows;
}

//Number of columns
int Matrix::GetNumberOfCols() const{
  return mNumCols;
}


//////////////////////////////////////////////////////
//OPERATORS OVERLOADING

//Parenthesis for indexation
double& Matrix::operator()(int i, int j){
  assert(i>0);
  assert(j>0);
  assert(i < mNumRows);
  assert(j < mNumCols);
  return mData[i][j];
}

//Equals
Matrix& Matrix::operator=(const Matrix& otherMatrix){
  assert(mNumRows == otherMatrix.mNumRows);
  assert(mNumCols == otherMatrix.mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
    mData[i][j] = otherMatrix.mData[i][j];
    }
  }
  return *this;
}

//Addition
Matrix Matrix::operator+() const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i,j) = mData[i][j];
    }
  }
  return m;
}
Matrix Matrix::operator+(const Matrix& m) const{
  assert(mNumRows == m.mNumRows);
  assert(mNumCols == m.mNumCols);
  Matrix mat(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      mat(i,j) = mData[i][j] + m.mData[i][j];
    }
  }
  return mat;
}

//Substraction
Matrix Matrix::operator-() const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i,j) = -mData[i][j];
    }
  }
  return m;
}
Matrix Matrix::operator-(const Matrix& m) const{
  assert(mNumRows == m.mNumRows);
  assert(mNumCols == m.mNumCols);
  Matrix mat(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      mat(i,j) = mData[i][j] - m.mData[i][j];
    }
  }
  return mat;
}

//Scalar multiplication
Matrix Matrix::operator*(double a) const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i,j) = a*mData[i][j];
    }
  }
  return m;
}

//Matrix multiplication
Matrix Matrix::operator*(const Matrix& m) const{
  //Dimension check
  assert(mNumRows == m.mNumCols);
  assert(mNumCols == m.mNumRows);
  //Computation
  int n = mNumRows;
  Matrix M(n, n);
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      double product = 0;
      for (int k = 0; k < mNumCols ; k++){
	product += mData[i][k] * m.mData[k][j];
      }
      M(i,j) = product;
    }
  }
  return M;
}


////////////////////////////////////////////////////
//OTHER METHODS

//Matrix vector multiplication
Vector operator*(const Matrix& m, const Vector& v){
  int orig_size = v.GetSize();
  int new_size = m.GetNumberOfRows();
  assert(m.GetNumberOfCols() == orig_size);
  Vector new_v(new_size);
  for(int i=0; i<new_size;i++){
    for(int j=0; j<orig_size; j++){
      new_v[i] += m.mData[i][j]*v.Read(j);
    }
  }
  return new_v;
}

// Vector matrix multiplication
Vector operator *(const Vector& v, const Matrix& m){
  int orig_size = v.GetSize();
  assert(m.GetNumberOfRows() == orig_size);
  int new_size = m.GetNumberOfCols();
  Vector new_v(new_size);
  for (int i=0; i<new_size; i++){
    for (int j=0; j<orig_size; j++){
      new_v[i] += v.Read(j)*m.mData[j][i];
    }
  }
  return new_v;
}
