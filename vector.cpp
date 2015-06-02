#include "vector.h"

#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

template<typename T>
vector<T> split(const string& line) {
  istringstream is(line);
  return vector<T>(istream_iterator<T>(is), istream_iterator<T>());
}

//Copy
Vector::Vector(const Vector& otherVector){
  mSize = otherVector.GetSize();
  mData = new double[mSize];
  for(int i=0; i<mSize; i++){
    mData[i] = otherVector.mData[i];
  }
}

//Size constructor
Vector::Vector(int size, double value){
  mData = NULL;
  assert(size>0);
  mSize = size;
  mData = new double[mSize];
  for (int i = 0; i<mSize; i++){
    mData[i] = value;
  }
}

//File constructor
Vector::Vector(string root) {
  mData = NULL;

  //Header reading
  string str;
  ifstream infile((root + "_H.data").c_str());
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    mSize = line[0];}

  //Vector reading
  FILE *vFile;
  mData = new double[mSize];
  vFile=fopen((root + "_V.bin").c_str(),"rb");
  if (!vFile){
    printf("Unable to open file!");
  }
  fread(mData,sizeof(*mData),mSize,vFile);
  fclose(vFile);
}

//Destructor
Vector::~Vector(){
  delete[] mData;
}


int Vector::GetSize() const{return mSize;}

double& Vector::operator[](int i){return mData[i];}

double Vector::Read(int i) const{return mData[i];}

Vector& Vector::operator=(const Vector& otherVector){
  assert(mSize == otherVector.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] = otherVector.mData[i];
  }
  return *this;
}

Vector Vector::operator+() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = mData[i];
  }
  return v;
}

Vector Vector::operator-() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = -mData[i];
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

Vector Vector::operator-(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] - v1.mData[i];
  }
  return v;
}

Vector Vector::operator*(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i]=a*mData[i];
  }
  return v;
}

void Vector::operator+=(const Vector& v1){
  assert(mSize == v1.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] += v1.mData[i];
  }
}

void Vector::operator-=(const Vector& v1){
  assert(mSize == v1.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] -= v1.mData[i];
  }
}

void Vector::operator*=(double a){
  for(int i=0;i<mSize;i++){
    mData[i] *= a;
  }
}


Vector Vector::inv() const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    if(mData[i] != 0){
      v[i] = 1.0 / mData[i];
    }
  }
  return v;
}

double Vector::norm(){
  double norm = 0.0;
  for(int i=0;i<mSize;i++){
    norm += mData[i] * mData[i];
  }
  norm = sqrt(norm);
  return norm;
}
