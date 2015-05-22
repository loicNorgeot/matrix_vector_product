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
  Vector(int size);
  Vector(std::string fileName);
  ~Vector();

  int GetSize() const;

  //Indexation à partir de 0 et de 1
  double& operator[](int i);
  double Read(int i) const;
  double& operator()(int i);

  //Surcharge d'opérateurs
  Vector& operator=(const Vector& otherVector);
  Vector operator-() const;
  Vector operator+() const;
  Vector operator-(const Vector& v1) const;
  Vector operator+(const Vector& v1) const;
  Vector operator*(double a) const;

  //Fonctions amies
  friend int length(const Vector& v);
  friend std::ostream& operator<<(std::ostream& output, const Vector& v);
};

int length(const Vector& v);
std::ostream& operator<<(std::ostream& output, const Vector& v);

#endif
