//Header de la classe matrix
#ifndef MATRIXHEADERDEF
#define MATRIXHEADERDEF

#include "vector.h"
#include <iostream>

class Matrix{
private:
  //Les données stockées (dynamique) et la taille de la matrice
  //Private pour ne modifier que par constructeur et méthodes
  double** mData;
  int mNumRows;
  int mNumCols;
public:
  //Un constructeur de copie
  Matrix(const Matrix& otherMatrix);
  //Un constructeur par taille
  Matrix(int rows, int cols);
  //Un destructeur
  ~Matrix();

  int GetNumberOfRows() const;
  int GetNumberOfCols() const;

  //Indexation à partir de 0 et de 1
  double& operator()(int i, int j);

  //Surcharge d'opérateurs
  Matrix& operator=(const Matrix& otherMatrix);
  Matrix operator-() const;
  Matrix operator+() const;
  Matrix operator-(const Matrix& m) const;
  Matrix operator+(const Matrix& m) const;
  Matrix operator*(double a) const;

  //Déterminant
  double CalculateDeterminant() const;

  //Multiplication par un vecteur
  friend Vector operator*(const Matrix& m, const Vector& v);
  friend Vector operator*(const Vector& v, const Matrix& m);
  friend std::ostream& operator<<(std::ostream& output, const Matrix& m);

  //Classe CSRmatrix définie comme amie pour avoir accès
  friend class CSRMatrix;
};

//Prototype des multiplications par vecteur
Vector operator*(const Matrix& m, const Vector& v);
Vector operator*(const Vector& v, const Matrix& m);
//prototype de la fonction de cout
std::ostream& operator<<(std::ostream& output, const Matrix& m);

#endif
