#include "vector.h"
#include "csrmatrix.h"

#include "omp.h"

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]){
  const int nP = atoi(argv[1]);
  double nbIter = 1;

  //Initialization
  string varName = "SIZE";
  string SIZE(getenv(varName.c_str()));
  string dataPath = "/work/norgeot/";
  CSRMatrix M(dataPath, SIZE, nP);
  Vector V(dataPath, SIZE);

  //Do nP computations
  for(int n = 1; n<=nP; n++){
    //if((n==1)||(n==2)||(n==4)||(n==8)||(n==16)||(n==32)||(n==64)||(n==128)){
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
      
      Vector result_vector(M.GetNumberOfRows());
      for (int i = 0 ; i < nbIter ; i++) {
	double start_time = omp_get_wtime();
	//Multiplication
	if(n==1){result_vector = M*V;}
	else{result_vector = parMult(M, V, n);}
	//Time recording and writing
	double final_time = omp_get_wtime() - start_time;
	log << final_time << endl;
	}   

      /*
      if(n==nP){
	ofstream res;
	string resName = "res.data";
	res.open(resName.c_str());
	cout << result_vector[10] << endl;
	for(int i = 0 ; i < M.GetNumberOfRows() ; i++){
	  res << result_vector[i] << endl;
	}
	res.close();
      }*/

      log.close();
      //}
  }

  return 0;
}

