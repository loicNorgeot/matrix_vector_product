#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF

#include <string>

using namespace std;

class Vector{

private:

  double* mData;
  int mSize;

public:

  //Constructors and destructors
  Vector(const Vector& otherVector);
  Vector(int size);
  Vector(int size, double value);
  Vector(string inPath, string name);
  ~Vector();

  //Accessing and assigning
  int GetSize() const;
  double& operator[](int i);
  double Read(int i) const;

  //Operators overloading
  //Equals
  Vector& operator=(const Vector& otherVector);
  //Addition
  Vector operator+() const;
  Vector operator+(const Vector& v1) const;
  Vector operator+(double a) const;
  void operator+=(const Vector& v1);
  //Substraction
  Vector operator-() const;
  Vector operator-(const Vector& v1) const;
  Vector operator-(double a) const;
  void operator-=(const Vector& v1);
  //Product
  Vector operator*(double a) const;
  void operator*=(double a);
  //Divide
  Vector operator/(double a) const;
  void operator/=(double a);
  //Scalar Product
  double operator*(const Vector& otherVector) const;

  //Other methods
  Vector inv() const;
  double norm();
  
};

#endif



