#include "vector.h"
#include "matrix.h"
#include "csrmatrix.h"

#include "omp.h"

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){

  //Number of processors recording
  const int nP = atoi(argv[1]);

  //Initialization
  double nbIter = 1;
  CSRMatrix M("newMatrix.data");
  Vector V("newVector.data");

  //Do nP computations
  for(int n = 1; n<=nP; n++){

    //Streams creation
    ofstream log;
    string n_str;
    stringstream convert;
    convert << n;
    n_str = convert.str();

    //Naming of time files as a function of the number of cores used
    string name;
    if (n<10){name = "computation_times_par_00" + n_str + ".txt";}
    else if ((n>9) && (n<100)){name = "computation_times_par_0" + n_str + ".txt";}
    else if (n>99){name = "computation_times_par_" + n_str + ".txt";}

    //File opening, and header writing
    log.open(name.c_str());
    log << M.GetNumberOfRows() << " " << M.GetNNZ() << " " << nP << endl;

    for (int i = 0 ; i < nbIter ; i++) {
      Vector result_vector(M.GetNumberOfRows());
      double start_time = omp_get_wtime();
      //Multiplication
      if(n==1){result_vector = M*V;}
      else{result_vector = parMult(M, V, n);}
      //Time recording and writing
      double final_time = omp_get_wtime() - start_time;
      log << final_time << endl;
    }

    log.close();
  }

  return 0;
}

