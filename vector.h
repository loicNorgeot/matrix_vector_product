//Header de la classe vector
#ifndef VECTORHEADERDEF
#define VECTORHEADERDEF

#include <iostream>
#include <string>

class Vector{
private:
  //Les données stockées (dynamique) et la taille du vecteur
  //Private pour ne modifier que par constructeur et méthodes
  double* mData;
  int mSize;
public:
  //Un constructeur de copie
  Vector(const Vector& otherVector);
  //Un constructeur par taille
  Vector(int size);
  //Un constructeur à partir d'un fichier (Cf CSRLoad.ipynb)
  Vector(std::string fileName);
  //Un destructeur
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

  //Norme
  double CalculateNorm(int p=2) const;

  //Fonctions amies
  friend int length(const Vector& v);
  friend std::ostream& operator<<(std::ostream& output, const Vector& v);
};

//Prototype de la fonction de longueur
int length(const Vector& v);
//prototype de la fonction de cout
std::ostream& operator<<(std::ostream& output, const Vector& v);

#endif
