#include "vector.h"
#include "csrmatrix.h"
#include "gc.h"

#include "omp.h"

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

void writeVectorData(const Vector& V, string fileName){
  ofstream out;
  out.open(fileName.c_str());
  for(int i = 0 ; i < V.GetSize() ; i++){
    out << V.Read(i) << endl;
  }
  out.close();
}

int main(int argc, char* argv[]){

  //Initialisation des variables
  const int nP = atoi(argv[1]);
  string varName = "SIZE";
  string SIZE(getenv(varName.c_str()));
  string dataPath = "/work/norgeot/";

  //Initialisation des données
  CSRMatrix A(dataPath + "matrix_" + SIZE, nP);
  Vector B(dataPath + "vector_" + SIZE);

  //Gradient conjugué
  
  //Vector RGC = GC(A, B, nP);
  //writeVectorData(RGC, "GC.data");
  
  //Gradient conjugué préconditionné par diagonale inverse
  
  Vector invDiag = (A.toDiagonal()).inv();
  Vector RGCP = GCPDiag(A, B, invDiag, nP);
  writeVectorData(RGCP, "test.data");

  return 0;
}


