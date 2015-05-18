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
std::vector<T> split(const std::string& line) {
  std::istringstream is(line);
  return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
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
Vector::Vector(int size){
  mData = NULL;
  assert(size>0);
  mSize = size;
  mData = new double[mSize];
  for (int i = 0; i<mSize; i++){
    mData[i] = 0.0;
  }
}

//File constructor
Vector::Vector(std::string fileName) {
  mData = NULL;
  //const std::string dataPath = "/work/norgeot/";

  /*
  //Méthode sur une ligne
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;
  while (std::getline(file, str)){
    std::vector<double> vect = split<double>(str);
    if(line == 1){
      mSize = vect.size();
      mData = new double[mSize];
      for(int i=0;i<mSize;i++){
        mData[i] = vect[i];
      }
    } 
    line++;
  }
  */

  /*
  //Méthode sur plusieurs lignes
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;
  while (std::getline(file, str)){
    if(line==0){
      std::vector<int> vect = split<int>(str);
      mSize = vect[0];
      mData = new double[mSize];
    }
    else{
      std::vector<double> vect = split<double>(str);
      mData[line-1] = vect[0];
    }
    line++;
  }
  */

  //Binary reading
  //File names
  string dataPath = "/work/norgeot/";
  string varName = "SIZE";
  string SIZE(std::getenv(varName.c_str()));
  string root = dataPath + "vector_" + SIZE;

  //Header reading
  std::string str;
  std::ifstream infile((root + "_H.data").c_str());
  while(std::getline(infile, str)){
    std::vector<int> line = split<int>(str);
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

//Size returns
int Vector::GetSize() const{
  return mSize;
}
//Size with length
int length(const Vector& v){
  return v.mSize;
}

//[] for indexation
double& Vector::operator[](int i){
  assert(i>-1);
  assert(i < mSize);
  return mData[i];
}

//read only bracket
double Vector::Read(int i) const
{
  assert(i > -1);
  assert(i < mSize);
  return mData[i];
}

//Parenthesis for indexation from 1
double& Vector::operator()(int i){
  assert(i>0);
  assert(i<mSize+1);
  return mData[i-1];
}

//Operator =
Vector& Vector::operator=(const Vector& otherVector){
  assert(mSize == otherVector.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] = otherVector.mData[i];
  }
  return *this;
}

//+self
Vector Vector::operator+() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = mData[i];
  }
  return v;
}

//-self
Vector Vector::operator-() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = -mData[i];
  }
  return v;
}

//a+b
Vector Vector::operator+(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] + v1.mData[i];
  }
  return v;
}
//a-b
Vector Vector::operator-(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] - v1.mData[i];
  }
  return v;
}

//Scalar multiplication
Vector Vector::operator*(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i]=a*mData[i];
  }
  return v;
}

// Euclidian norm
double Vector::CalculateNorm(int p) const{
  double norm_val, sum = 0.0;
  for (int i=0; i<mSize; i++){
    sum += pow(fabs(mData[i]), p);
  }
  norm_val = pow(sum, 1.0/((double)(p)));
  return norm_val;
}

//Printing
std::ostream& operator<<(std::ostream& output, const Vector& v){
  std::string sep = " ";
  output << "(";
  if (v.mSize<10){
    for (int i=0; i<v.mSize;i++){
      output << v.mData[i] << sep;
    }
  }
  else{
    output << v.mData[0] << sep << v.mData[1] << " ... " << v.mData[v.mSize-2] << sep << v.mData[v.mSize - 1];
  }
  output << ")" << std::endl;
}
