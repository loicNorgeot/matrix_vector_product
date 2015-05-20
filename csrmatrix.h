//Header de la classe CSRMatrix
#ifndef CSRMATRIXHEADERDEF
#define CSRMATRIXHEADERDEF

#include "vector.h"
#include "matrix.h"

class CSRMatrix{
private:
  double *mA;
  int *mIA;
  int *mJA;
  int mNNZ;
  int mNumRows;
public:
  //Un constructeur de copie
  CSRMatrix(const CSRMatrix& otherMatrix);
  //Un constructeur à partir d'une matrice de la classe Matrix
  CSRMatrix(const Matrix& M);
  //Un constructeur à partir d'un fichier (Cf CSRLoad.ipynb)
  CSRMatrix(std::string fileName, const int nbProcs);
  //Un destructeur
  ~CSRMatrix();

  int GetNNZ() const;
  int GetNumberOfRows() const;
  double GetA(int i) const;
  int GetIA(int i) const;
  int GetJA(int i) const;

  friend std::ostream& operator<<(std::ostream& output, const CSRMatrix& m); 
  friend Vector operator*(const CSRMatrix& m, const Vector& v);
  friend Vector parMult(const CSRMatrix& m, const Vector& v, const int nbProcs);
};

//Prototypes des fonctions amies
std::ostream& operator<<(std::ostream& output, const CSRMatrix& m);
Vector operator*(const CSRMatrix& m, const Vector& v);
Vector parMult(const CSRMatrix& m, const Vector& v, const int nbProcs);

#endif
