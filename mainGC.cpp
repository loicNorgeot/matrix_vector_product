#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

#include "vector.h"
#include "csrmatrix.h"
#include "gc.h"

#include "omp.h"

using namespace std;

int main(int argc, char* argv[]){
  //Variables
  const int nP = atoi(argv[1]);
  string varName = "SIZE";
  string SIZE(getenv(varName.c_str()));
  string dataPath = "/work/norgeot/";

  //Initialization
  CSRMatrix A(dataPath, SIZE, nP);
  Vector B(dataPath, SIZE);

  //Conjugate Gradient
  //Vector RGC = GC(A, B, nP);
  //RGC.write("GC.data");
  Vector invDiag = (A.toDiagonal()).inv();
  Vector RGCP = PGC(A, B, invDiag, nP);
  RGCP.write("test.data");

  return 0;
}


