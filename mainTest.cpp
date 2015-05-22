#include "vector.h"
#include "matrix.h"
#include "csrmatrix.h"
#include "gc.h"

#include "omp.h"

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
  //PARSING
  const int nP = atoi(argv[1]);

  //INITIALIZING
  CSRMatrix M("newMatrix.data",nP);
  Vector V("newVector.data");
  Vector R(V.GetSize());

  //MONITORING
  ofstream log;
  string n_str;
  stringstream convert;
  convert << nP;
  n_str = convert.str();
  log.open("TIME.txt".c_str());

  //COMPUTING
  double t0=omp_get_wtime(), t1=0;
  R = GC(M, V, nP);
  t1 = omp_get_wtime() - t0;

  //CLOSING
  log << "Temps d'execution = " << t1 << " s." << endl;
  log.close();
  return 0;
}
