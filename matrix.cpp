#include "matrix.h"

#include <cmath>
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

//Copy constructor
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

//Size constructor
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

//Size returning
int Matrix::GetNumberOfRows() const{
  return mNumRows;
}
int Matrix::GetNumberOfCols() const{
  return mNumCols;
}

//Overloading parenthesis for indexation
double& Matrix::operator()(int i, int j){
  assert(i>0);
  assert(j>0);
  assert(i < mNumRows + 1);
  assert(j < mNumCols + 1);
  return mData[i-1][j-1];
}

//Overloading =
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

//Overloading +self
Matrix Matrix::operator+() const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i+1,j+1) = mData[i][j];
    }
  }
  return m;
}
//Overloading -self
Matrix Matrix::operator-() const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i+1,j+1) = -mData[i][j];
    }
  }
  return m;
}

//Overloading binary addition
Matrix Matrix::operator+(const Matrix& m) const{
  assert(mNumRows == m.mNumRows);
  assert(mNumCols == m.mNumCols);
  Matrix mat(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      mat(i+1,j+1) = mData[i][j] + m.mData[i][j];
    }
  }
  return mat;
}
//Overloading binary substraction
Matrix Matrix::operator-(const Matrix& m) const{
  assert(mNumRows == m.mNumRows);
  assert(mNumCols == m.mNumCols);
  Matrix mat(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      mat(i+1,j+1) = mData[i][j] - m.mData[i][j];
    }
  }
  return mat;
}

//Scalar multiplication
Matrix Matrix::operator*(double a) const{
  Matrix m(mNumRows, mNumCols);
  for(int i=0;i<mNumRows;i++){
    for(int j=0;j<mNumCols;j++){
      m(i+1,j+1) = a*mData[i][j];
    }
  }
  return m;
}

//Determinant computation
double Matrix::CalculateDeterminant() const{
  assert(mNumCols == mNumRows);
  double determinant = 0.0;
  if (mNumRows == 1){
    determinant = mData[0][0];
  }
  else{
    for (int i_outer=0; i_outer<mNumRows; i_outer++){
	Matrix sub_matrix(mNumRows-1, mNumRows-1);
	for (int i=0; i<mNumRows-1; i++){
	    for (int j=0; j<i_outer; j++){
		sub_matrix(i+1,j+1) = mData[i+1][j];
	    }
	    for (int j=i_outer; j<mNumRows-1; j++){
	      sub_matrix(i+1,j+1) = mData[i+1][j+1];
	    }
	}
	double sub_matrix_determinant = sub_matrix.CalculateDeterminant();
	determinant += pow(-1.0, i_outer)*mData[0][i_outer]*sub_matrix_determinant;
    }
  }
  return determinant;
}

//Print
std::ostream& operator<<(std::ostream& output, const Matrix& m){
  //Classification according to number of rows, then number of cols
  std::string sep = " ";
  if (m.mNumRows<10){
    if (m.mNumCols<10){
      for (int i=0; i<m.mNumRows;i++){
	for (int j=0; j<m.mNumCols;j++){
	  output << m.mData[i][j] << sep;
	}
	output << std::endl;
      }
    }
    else{
      for (int i=0; i<m.mNumRows;i++){
	output << m.mData[i][0] << sep << m.mData[i][1] << " ... " << m.mData[i][m.mNumCols - 2] << sep << m.mData[i][m.mNumCols - 1];
	output << std::endl;
      }
    }
  }
  else{
    if (m.mNumCols<10){
      for (int i=0; i<2;i++){
	for (int j=0; j<m.mNumCols;j++){
	  output << m.mData[i][j] << sep;
	}
	output << std::endl;
      }
      output << "..." << std::endl;
      for (int i=m.mNumRows - 2; i<m.mNumRows;i++){
	for (int j=0; j<m.mNumCols;j++){
	  output << m.mData[i][j] << sep;
	}
	output << std::endl;
      }
    }
    else{
      for (int i=0; i<2;i++){
	output << m.mData[i][0] << sep << m.mData[i][1] << "..." << m.mData[i][m.mNumCols-2] << sep << m.mData[i][m.mNumCols - 1];
	output << std::endl;
      }
      output << "..." << std::endl;
      for (int i=m.mNumRows-2; i<m.mNumRows;i++){
	output << m.mData[i][0] << sep << m.mData[i][1] << "..." << m.mData[i][m.mNumCols-2] << sep << m.mData[i][m.mNumCols - 1];
	output << std::endl;
      }
    }
  }
}

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
Vector operator *(const Vector& v, const Matrix& m)
{
  int orig_size = v.GetSize();
  assert(m.GetNumberOfRows() == orig_size);
  int new_size = m.GetNumberOfCols();
  Vector new_v(new_size);
  for (int i=0; i<new_size; i++)
    {
      for (int j=0; j<orig_size; j++)
	{
	  new_v[i] += v.Read(j)*m.mData[j][i];
	}
    }
  return new_v;
}
