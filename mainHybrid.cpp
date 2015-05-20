#include "vector.h"
#include "matrix.h"
#include "csrmatrix.h"

#include "mpi.h"
#include "omp.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(){
  /*---------------------
      INITIALIZATION
  ---------------------*/
  MPI::Init();
  CSRMatrix M("/work/norgeot/matrix.data");
  Vector V("/work/norgeot/vector.data");
  const int nR = M.GetNumberOfRows();

  const int s = MPI::COMM_WORLD.Get_size();
  const int id = MPI::COMM_WORLD.Get_rank();
  const int chunk_size = (int)( ceil(nR/s));

  /*-------------------
    CALCUL DU PRODUIT
  -------------------*/
  // Result array initialization
  double *sol=NULL;
  sol = new double[chunk_size];
  for(int i=0; i<chunk_size ;i++){sol[i]=0;}

  //v_copy creation
  double *v_copy = NULL;
  v_copy = new double[nR];
#pragma omp parallel for schedule(static,chunk_size/8)
  for (int i = 0; i< nR; i++){
    v_copy[i] = V.Read(i);
  }

  //Synchronization
  MPI_Barrier(MPI::COMM_WORLD);
  double t0 = omp_get_wtime();
  double tf = 0;
  double tTot=0;
  
  // OpenMP parallel computation
#pragma omp parallel for schedule(static,chunk_size/8)// firstprivate(v_copy)
  for(int i=0; i<chunk_size; i++){
    double s_temp = 0;
    for(int j=M.GetIA(id*chunk_size+i); j<M.GetIA(id*chunk_size+i+1); j++){
      s_temp += M.GetA(j)*v_copy[M.GetJA(j)];
    }
    sol[i] = s_temp;
  }
  tf = omp_get_wtime() - t0;
  cout << "Rang " << id << ": " << tf << " s." <<endl; 
  
  //Synchronization
  MPI_Barrier(MPI::COMM_WORLD);
  if(id==0){
    tTot = omp_get_wtime() - t0;
    cout << "Temps total d'éxécution = " << tTot << " s." << endl;
    cout << "Chunk de taille " << chunk_size << " pour " << nR << " colonnes." << endl;
  }
  delete[] v_copy;

  // Sending sol to process 0
  if(id!=0){
    int tag_send = id*10;
    MPI_Send(sol, chunk_size, MPI_DOUBLE, 0, tag_send, MPI::COMM_WORLD);
  }


  /*----------------------------------------------
           Results reading from process  0
    ----------------------------------------------*/
  

  if(id==0){  
    // Final array initialization
    double *final_res = NULL;
    final_res = new double[nR];
    for(int i = 0 ; i < nR ; i++){ 
      final_res[i] = 0;
    }

    // Receiving incoming messages
    for(int i = 1 ; i < s; i++){
      MPI_Status status;
      int tag_receive = i*10;
      double *buff = NULL;
      buff = new double[chunk_size];
      MPI_Recv(buff, chunk_size, MPI_DOUBLE, i, tag_receive, MPI::COMM_WORLD, &status);
      // Storing in final_res
      for (int j = 0 ; j<chunk_size ; j++){
	final_res[i*chunk_size+j]+=buff[j];
      }
      delete[] buff;
    }

    // Storing for process 0
    for (int j = 0 ; j < chunk_size ; j++){
      final_res[j]+=sol[j];
    }

    // Writing the result
    ofstream fout("res.data");
    if(fout.is_open()){
      cout << "File opened" << endl;
      for(int i = 0; i<nR; i++){
	fout << final_res[i] << endl;
      }
    }
    else{cout << "File not opened" << endl;}
    delete[] final_res;
  }


  delete[] sol;
  MPI::Finalize();
  return 0;
}
