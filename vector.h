//Header de la classe vector
#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF

#include <iostream>
#include <string>

class Vector{

private:
  double* mData;
  int mSize;

public:
  Vector(const Vector& otherVector);
  Vector(int size, double value=0.0);
  Vector(std::string inPath, std::string name);
  ~Vector();

  int GetSize() const;

  double& operator[](int i);
  double Read(int i) const;

  Vector& operator=(const Vector& otherVector);
  Vector operator-() const;
  Vector operator+() const;
  Vector operator-(const Vector& v1) const;
  Vector operator+(const Vector& v1) const;
  void operator-=(const Vector& v1);
  void operator+=(const Vector& v1);
  void operator*=(double a);
  Vector operator*(double a) const;

  Vector inv() const;
  double norm();
};

#endif
