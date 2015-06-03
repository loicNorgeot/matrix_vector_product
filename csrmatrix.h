#ifndef CSRMATRIXHEADERDEF
#define CSRMATRIXHEADERDEF

#include <string>
#include "vector.h"

using namespace std;

class CSRMatrix{

private:

  double *mA;
  unsigned int *mIA;
  int *mJA;
  unsigned int mNNZ;
  int mNumRows;

public:

  //Constructors and destructors
  CSRMatrix(string inPath, string name, const int nbProcs);
  ~CSRMatrix();

  //Private variables accessing
  unsigned int GetNNZ() const;
  int GetNumberOfRows() const;
  unsigned int GetIA(int i) const;
  int GetJA(int i) const;
  double GetA(int i) const;

  //Matrix Vector product
  Vector operator*(const Vector& v) const;
  Vector parallelProduct(const Vector& v, const int nbProcs) const;

  //Other methods
  Vector toDiagonal() const;

};

#endif
