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

//From a Vector Object
Matrix::Matrix(const Vector& V){
  mNumRows = V.GetSize();
  mNumCols = 1;
  mData = new double*[mNumRows];
  for(int i=0; i<mNumRows; i++){
    mData[i] = new double[mNumCols];
  }
  for(int i=0; i<mNumRows; i++){
    mData[i][0] = V.Read(i);
  }
}

//Size
Matrix::Matrix(int size){
  assert(size>0);
  mNumRows = size;
  mNumCols = size;
  mData = new double*[mNumRows];
  for(int i=0; i<mNumRows; i++){
    mData[i] = new double[mNumCols];
  }
  for(int i=0; i<mNumRows; i++){
    for(int j=0; j<mNumCols; j++){
      mData[i][j] = 0.0;
    }
  }
}

//Size
Matrix::Matrix(int rows, int cols){
  assert(rows>0);
  assert(cols>0);
  mNumRows = rows;
  mNumCols = cols;
  mData = new double*[mNumRows];
  for(int i=0; i<mNumRows; i++){
    mData[i] = new double[mNumCols];
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
//SIZE MODIFICATIONS, APPENDING...

//Resizing the whole matrix
void Matrix::resize(int newRows, int newCols){
  int colsToAdd = newCols - mNumCols;
  int rowsToAdd = newRows - mNumRows;

  //Temporary array declaration
  double **newData = new double*[newRows];
  for (int j = 0 ; j < newRows ; j++){
    newData[j] = new double[newCols];
  }
  //Temporary array initialization
  int resizeRows=0, resizeCols=0;
  if(colsToAdd>=0){resizeCols = mNumCols;}
  else{resizeCols = newCols;}
  if(rowsToAdd>=0){resizeRows = mNumRows;}
  else{resizeRows = newRows;}
  for (int i = 0 ; i < mNumRows ; i++){
    if (i < resizeRows){
      for (int j = 0 ; j < resizeCols ; j++){
	newData[i][j] = mData[i][j];
      }
    }
    delete[] mData[i];
  }
  delete[] mData;
  //Adding 0 in the newly created spaces
  for (int i = mNumRows ; i < newRows ; i++){
    for(int j = 0 ; j < newCols ; j++){
      newData[i][j] = 0.0;
    }
  }
  for (int i = 0 ; i < mNumRows ; i++){
    for(int j = mNumCols ; j < newCols ; j++){
      newData[i][j] = 0.0;
    }
  }
  //Recording the changes
  mData = newData;
  mNumCols = newCols;
  mNumRows = newRows;
}

//Deleting rows
void Matrix::delRow(int nRowsToDel){this->resize(mNumRows-nRowsToDel, mNumCols);}

//Adding rows
void Matrix::addRow(int nRowsToAdd){this->resize(mNumRows+nRowsToAdd, mNumCols);}

//Adding columns
void Matrix::addCol(int nColsToAdd){this->resize(mNumRows, mNumCols+nColsToAdd);}

//Deleting columns
void Matrix::delCol(int nColsToDel){this->resize(mNumRows, mNumCols-nColsToDel);}

//Adding a new row from a vector
void Matrix::addRow(const Vector& newRow){
  assert(newRow.GetSize() == mNumCols);
  this->addRow();
  for(int j = 0 ; j < mNumCols ; j++){
    mData[mNumRows][j] = newRow.Read(j);
  }
}

//Adding a new column from a vector
void Matrix::addCol(const Vector& newCol){
  assert(newCol.GetSize() == mNumRows);
  this->addCol();
  for(int i = 0 ; i < mNumRows ; i++){
    mData[i][mNumCols] = newCol.Read(i);
  }
}


////////////////////////////////////////////////////
//TO AND FROM VECTOR

//Row extraction
Vector Matrix::row(int row){
  Vector V(mNumCols);
  for(int i = 0 ; i < mNumCols ; i++){
    V[i] = mData[row][i];
  }
  return V;
}

//Column extraction
Vector Matrix::col(int col){
  Vector V(mNumRows);
  for(int i = 0 ; i < mNumRows ; i++){
    V[i] = mData[i][col];
  }
  return V;
}


////////////////////////////////////////////////////
//IDENTITY AND SCALAR IDENTITY

//Identity or scalar
Matrix ID(int size, double val){
  Matrix M(size, size);
  for(int i=0; i<M.mNumRows; i++){
    for(int j=0; j<M.mNumCols; j++){
      if(i==j){
	M.mData[i][j] = val;
      }
    }
  }
  return M;
}

//Identity or scalar from a Vector
Matrix ID(const Vector& V){
  assert(V.GetSize()>0);
  Matrix M(V.GetSize(), V.GetSize());
  for(int i=0; i<M.mNumRows; i++){
    for(int j=0; j<M.mNumCols; j++){
      if(i==j){
	M.mData[i][j] = V.Read(i);
      }
    }
  }
  return M;
}


////////////////////////////////////////////////////
//VECTOR MULTIPLICATION

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
