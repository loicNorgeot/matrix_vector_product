//Header de la classe linearsystem
#ifndef LINEARSYSTEMHEADERDEF
#define LINEARSYSTEMHEADERDEF
#include "vector.h"
#include "matrix.h"

class LinearSystem
{
 private:
  int mSize;
  Matrix* mpA;
  Vector* mpb;
  LinearSystem(const LinearSystem& otherLinearSystem){};//Déclaré vide pour éviter toute copie
public:
  LinearSystem(const Matrix& A, const Vector& b);
  ~LinearSystem();
  virtual Vector Solve();
};

#endif

