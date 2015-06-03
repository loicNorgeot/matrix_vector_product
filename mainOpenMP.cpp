#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string>

#include "vector.h"
#include "csrmatrix.h"

#include "omp.h"

using namespace std;

int main(int argc, char* argv[]){

  //Variables
  const int nP = atoi(argv[1]);
  double nbIter = 1;

  //Initialization
  string varName = "SIZE";
  string SIZE(getenv(varName.c_str()));
  string dataPath = "/work/norgeot/";
  CSRMatrix M(dataPath, SIZE, nP);
  Vector V(dataPath, SIZE);

  //Compute a product for each number of processors under nP
  for(int n = 1; n<=nP; n++){

    //Streams
    ofstream log;
    string n_str;
    stringstream convert;
    convert << n;
    n_str = convert.str();

    //Naming of time files
    string name;
    if (n<10){name = "computation_times_par_00" + n_str + ".txt";}
    else if ((n>9) && (n<100)){name = "computation_times_par_0" + n_str + ".txt";}
    else if (n>99){name = "computation_times_par_" + n_str + ".txt";}

    //File opening, and header writing
    log.open(name.c_str());
    log << M.GetNumberOfRows() << " " << M.GetNNZ() << " " << nP << endl;

    //Actual computation
    Vector result_vector(M.GetNumberOfRows());
    for (int i = 0 ; i < nbIter ; i++) {
      double start_time = omp_get_wtime();
      if(n==1){result_vector = M*V;}
      else{result_vector = M.parallelProduct(V, nP);}
      double final_time = omp_get_wtime() - start_time;
      log << final_time << endl;
    }   
    if(n == nP){result_vector.write("result_vector.data");}
    log.close();
  }
  return 0;
}

