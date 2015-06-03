#include <cmath>
#include <cassert>
#include <string>

#include "vector.h"
#include "binaryIO.h"

using namespace std;


///////////////////////////////////////////
//CONSTRUCTORS & DESTRUCTOR

//Copy
Vector::Vector(const Vector& otherVector){
  mSize = otherVector.GetSize();
  mData = new double[mSize];
  for(int i=0; i<mSize; i++){
    mData[i] = otherVector.mData[i];
  }
}

//Size
Vector::Vector(int size){
  mData = NULL;
  assert(size>0);
  mSize = size;
  mData = new double[mSize];
  for (int i = 0; i<mSize; i++){
    mData[i] = 0;
  }
}

//Size and Value
Vector::Vector(int size, double value){
  mData = NULL;
  assert(size>0);
  mSize = size;
  mData = new double[mSize];
  for (int i = 0; i<mSize; i++){
    mData[i] = value;
  }
}

//File
Vector::Vector(string inPath, string name) {
  mData = NULL;
  int nR=0;
  unsigned int nnz=0;
  getHeaderInfo(nR, nnz, inPath, name);
  mSize = nR;
  mData = new double[nR];
  brVector(mData, nR, inPath, name);
}

//Destructor
Vector::~Vector(){
  delete[] mData;
}


///////////////////////////////////////////////////
//READING AND EVALUATING

//Size return
int Vector::GetSize() const{return mSize;}

//Assigning and reading
double& Vector::operator[](int i){return mData[i];}

//Read only
double Vector::Read(int i) const{return mData[i];}


//////////////////////////////////////////////////////
//OPERATORS OVERLOADING

//Equals
Vector& Vector::operator=(const Vector& otherVector){
  assert(mSize == otherVector.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] = otherVector.mData[i];
  }
  return *this;
}

//Addition
Vector Vector::operator+() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = mData[i];
  }
  return v;
}
Vector Vector::operator+(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] + v1.mData[i];
  }
  return v;
}
Vector Vector::operator+(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] + a;
  }
  return v;
}
void Vector::operator+=(const Vector& v1){
  assert(mSize == v1.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] += v1.mData[i];
  }
}

//Substraction
Vector Vector::operator-() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = -mData[i];
  }
  return v;
}
Vector Vector::operator-(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] - v1.mData[i];
  }
  return v;
}
Vector Vector::operator-(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] - a;
  }
  return v;
}
void Vector::operator-=(const Vector& v1){
  assert(mSize == v1.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] -= v1.mData[i];
  }
}

//Product
Vector Vector::operator*(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i]=a*mData[i];
  }
  return v;
}
void Vector::operator*=(double a){
  for(int i=0;i<mSize;i++){
    mData[i] *= a;
  }
}

//Division
Vector Vector::operator/(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i]=mData[i]/a;
  }
  return v;
}
void Vector::operator/=(double a){
  for(int i=0;i<mSize;i++){
    mData[i] /= a;
  }
}

//Scalar product
double Vector::operator*(const Vector& otherVector) const{
  assert(mSize == otherVector.mSize);
  double scp = 0.0;
  for(int i=0;i<mSize;i++){
    scp += mData[i] * otherVector.mData[i];
  }
  return scp;
}


////////////////////////////////////////////
//OTHER METHODS

//Inverse
Vector Vector::inv() const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    if(mData[i] != 0){
      v[i] = 1.0 / mData[i];
    }
  }
  return v;
}

//Norm
double Vector::norm(){
  double norm = 0.0;
  for(int i=0;i<mSize;i++){
    norm += mData[i] * mData[i];
  }
  norm = sqrt(norm);
  return norm;
}
