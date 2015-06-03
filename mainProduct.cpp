//Prend en argument le nombre de processeurs souhaités
#include "omp.h"
#include "binaryIO.h"
#include "chrono.h"

#include <string>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]){

  const int nP = atoi(argv[1]);
  string inPath = "/work/norgeot/";
  string varName = "SIZE";
  string name(getenv(varName.c_str()));
  double t = omp_get_wtime();
  int compteur = 0;

  // ----------------------------------------
  //                LECTURE
  // ----------------------------------------
  //Déclaration et variables
  int nR = 0;
  unsigned int nnz = 0;
  getHeaderInfo(nR, nnz, inPath, name);
  t = chrono(t, compteur, "Lecture du header");

  //Initialisation
  unsigned int *IAt = new unsigned int[nR + 1];
  int *JAt = new int[nnz];
  double *At = new double[nnz];
  double *V = new double[nR];
  t = chrono(t, compteur, "Initialisation des tableaux temporaires");

  //Lecture
  brMatrix(IAt, JAt, At, nR, nnz, inPath, name);
  brVector(V, nR, inPath, name);
  t = chrono(t, compteur, "Lecture des fichiers binaires");
 
  //Définition du chunk
  const int chunk = nR/nP;

  // ----------------------------------------
  //              FIRST TOUCH
  // ----------------------------------------
  //Initialisation
  unsigned int *IA = new unsigned int[nR + 1];
  int *JA = new int[nnz];
  double *A = new double[nnz];
  t = chrono(t, compteur, "Initialisation des tableaux finaux");

  //Initialisation en parallèle
#pragma omp parallel for num_threads(nP) schedule(static, chunk)
  for (int i = 0 ; i < nR; i++ ){
    IA[i] = IAt[i];
    IA[i+1] = IAt[i+1];
    for(unsigned int j=IA[i]; j<IA[i+1]; j++){
      A[j] = At[j];
      JA[j] = JAt[j];
    }
  }
  t = chrono(t, compteur, "First touch");

  //Desallocation
  delete[] IAt;
  delete[] JAt;
  delete[] At;
  t = chrono(t, compteur, "Désallocation des tableaux temporaires");

  // ----------------------------------------
  //             PRODUIT
  // ----------------------------------------

  ///////////////////////////////////////////
  //VERSION1
  //Initialisation du résultat
  double *sol = new double[nR];

  //Copie locale du vecteur
  double *v_copy=NULL;
  v_copy = new double[nR];
#pragma omp for schedule(static, chunk)
  for(int i=0; i<nR; i++){
    v_copy[i] = V[i];
  }
  t = chrono(t, compteur, "Copie locale du vecteur");

  //Calcul du produit
#pragma omp parallel for num_threads(nP) schedule(static, chunk) firstprivate(v_copy)
  for(int i=0; i<nR; i++){
    double s_temp = 0.0;
    for(unsigned int j=IA[i]; j<IA[i+1]; j++){
      s_temp += A[j]*v_copy[JA[j]];
    }
    sol[i] = s_temp;
  }
  t = chrono(t, compteur, "Produit parallèle");
  delete[] v_copy;

  // ----------------------------------------
  //             NETTOYAGE
  // ----------------------------------------
  delete[] V;
  delete[] A;
  delete[] IA;
  delete[] JA;
  t = chrono(t, compteur, "Désallocation finale");

  // ----------------------------------------
  //             AFFICHAGE
  // ----------------------------------------
  cout << "SOL = " << sol[0] << ", " << sol[1] << " ... " << sol[nR-2] << ", " << sol[nR-1] << endl;
  delete[] sol;
}
