#include "vector.h"
#include "matrix.h"
#include "linearsystem.h"
#include "csrmatrix.h"
#include "fonctions_main.h"

#include "omp.h"
#include <cstdlib>
#include "time.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
  //(atoi(argv[1]), atof(argv[2]), atoi(argv[3])) pour les arguments
  //Test du produit avec les matrices lues dans les ".data"
  launchParallelCSRProductOnData(atoi(argv[1]));
  return 0;
}

