#include "mpi.h"
#include "omp.h"
#include <iostream>
#include <cmath>
#include <fstream>

#include "vector.h"
#include "matrix.h"
#include "csrmatrix.h"
#include "linearsystem.h"
#include "fonctions_main.h"

using namespace std;

int main(){
  /*---------------------
      INITIALISATION
  ---------------------*/
  MPI::Init();
  // Initialisation des variables
  CSRMatrix M("/work/norgeot/matrix.data");
  Vector V("/work/norgeot/vector.data");
  const int nR = M.GetNumberOfRows();

  //Création des variables liées à MPI
  const int s = MPI::COMM_WORLD.Get_size();
  const int id = MPI::COMM_WORLD.Get_rank();
  int chunk_size = (int)( ceil(nR/s));

  /*
  // Calcul des bornes pour la décomposition MPI
  int down, up;
  if(id!=s-1){
    down = id*chunk_size;
    up = (id+1)*chunk_size;
  }
  else{
    down = id*chunk_size;
    up = nR;
  }
  cout << nR << endl;
  cout << "Rang " << id << ", lower = " << down << ", upper = " << up << endl; 
  */

  /*-------------------
    CALCUL DU PRODUIT
  -------------------*/
  // Création du tableau de retour
  double *sol=NULL;
  sol = new double[chunk_size];
  for(int i=0; i<chunk_size ;i++){sol[i]=0;}

  double *v_copy = NULL;
  v_copy = new double[nR];
#pragma omp parallel for schedule(dynamic,1000)
  for (int i = 0; i< nR; i++){
    v_copy[i] = V.Read(i);
  }

  //Synchronisation de début de calcul
  MPI_Barrier(MPI::COMM_WORLD);
  double t0 = omp_get_wtime();
  double tf = 0;
  double tTot=0;
  
  // Calcul parallèle en OpenMP
#pragma omp parallel for schedule(dynamic,1000)
  for(int i=0; i<chunk_size; i++){
    double s_temp = 0;
    for(int j=M.GetIA(id*chunk_size+i); j<M.GetIA(id*chunk_size+i+1); j++){
      s_temp += M.GetA(j)*v_copy[M.GetJA(j)];
    }
    sol[i] = s_temp;
  }
  
  //Synchronisation de fin de calcul
  tf = omp_get_wtime() - t0;
  cout << "Temps d'éxécution pour le rang " << id << " = " << tf << " s." <<endl; 
  MPI_Barrier(MPI::COMM_WORLD);
  if(id==0){
    tTot = omp_get_wtime() - t0;
    cout << "Temps total d'éxécution = " << tTot << " s." << endl;
  }
  delete[] v_copy;

  // Envoi de l'adresse de sol au process 0
  if(id!=0){
    int tag_send = id*10;
    MPI_Send(sol, chunk_size, MPI_DOUBLE, 0, tag_send, MPI::COMM_WORLD);
  }


  /*----------------------------------------------
    LECTURE DES RESULTATS A PARTIR DU PROCESS 0
    ----------------------------------------------*/
  // Initialisation du tableau final
  double *final_res = NULL;
  final_res = new double[nR];
  for(int i = 0 ; i < nR ; i++){ 
    final_res[i] = 0;
  }

  if(id==0){  
    // Lecture des messages entrants
    for(int i = 1 ; i < s; i++){
      MPI_Status status;
      int tag_receive = i*10;
      double *buff = NULL;
      buff = new double[chunk_size];
      MPI_Recv(buff, chunk_size, MPI_DOUBLE, i, tag_receive, MPI::COMM_WORLD, &status);
      // Ecriture dans final_res
      for (int j = 0 ; j<chunk_size ; j++){
	final_res[i*chunk_size+j]+=buff[j];
      }
      delete[] buff;
    }

    // Enregistrement de sol dans temp pour le rang 0
    for (int j = 0 ; j < chunk_size ; j++){
      final_res[j]+=sol[j];
    }
  }


  /*--------------------------
           DEBUGGAGE
  --------------------------*/
  if(id==0){
    // Comptage du nombre d'éléments nuls
    int compteur = 0;
    for (int i = 0 ; i < nR ; i++){
      if(final_res[i]<1e-6){
	compteur++;
      }
    }
    cout << "Nombre d'éléments nuls dans le résultat final = " << compteur << endl;

    // Ecriture du fichier résultant
    ofstream fout("res.data");
    if(fout.is_open()){
      cout << "Ouverture réussie" << endl;
      for(int i = 0; i<nR; i++){
	fout << final_res[i] << endl;
      }
      cout << "Ecriture réussie" << endl;
    }
    else{
      cout << "Problème d'ouverture" << endl;
    }
    delete[] final_res;
  }

  delete[] sol;

  //Fin du programme
  MPI::Finalize();
  return 0;
}
