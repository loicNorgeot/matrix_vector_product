#include "fonctions_main.h"
#include "omp.h"
#include <cstdlib>
#include "time.h"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "vector.h"
#include "matrix.h"
#include "linearsystem.h"
#include "csrmatrix.h"

using namespace std;

Matrix createBigMatrix(int size, double ratio){
  //ratio = rapport entre le nombre d'éléments et le nombre de non zéros
  Matrix bigMatrix(size,size);
  int NNZ = ratio * size;
  cout << NNZ << endl;
  srand(time(NULL));
  for(int i=0; i<size;i++){
    for(int j=0; j<NNZ;j++){
      int out = rand() % size;
      bigMatrix(i+1,out+1) = rand()%10;
    }
  }
  return bigMatrix;
}

void testVectorClass(){
  //Test de la classe vecteurs
  int s = 3;
  Vector v1(s), v2(s);
  v1[0]=1;
  v2[0] = 2;
  std::cout << (v2 + v1)*3.0;
}

void testMatrixClass(){
  //Test de la classe matrix
  int r = 3, c = 3;
  Matrix m1(r,c), m2(r,c);
  m1(1,2) = 12;
  m2(2,1) = 21;
  std::cout << m1;
  std::cout << " " << std::endl;
  std::cout << m2;
  std::cout << " " << std::endl;
  std::cout << m1 - m2;
}

void testMatrixVectorProduct(){
  //Test des multiplications
  Matrix m1(3,3);
  Vector v1(3);
  m1(1,1) = 1;
  m1(1,2) = 1;
  m1(1,3) = 1;
  m1(2,2) = 2;
  m1(2,3) = 5;
  m1(3,1) = 2;
  m1(3,2) = 5;
  m1(3,3) = -1;
  v1[0] = 6;
  v1[1] = -4;
  v1[2] = 27;
  std::cout << m1*v1;
}

void testLinearSystem(){
  //Test d'un système linéaire
  Matrix m1(3,3);
  Vector v1(3);
  m1(1,1) = 1;
  m1(1,2) = 1;
  m1(1,3) = 1;
  m1(2,2) = 2;
  m1(2,3) = 5;
  m1(3,1) = 2;
  m1(3,2) = 5;
  m1(3,3) = -1;
  v1[0] = 6;
  v1[1] = -4;
  v1[2] = 27;
  LinearSystem L(m1,v1);
  Vector solution(3);
  solution = L.Solve();
  std::cout<<std::endl<<"Solution du système linéaire ="<<std::endl;
  std::cout << solution;
  std::cout << std::endl;
}

void launchParallelCSRProductOnRandomMatrix(int size, double ratio, int numProcs){
  //Test de la classe CSRMatrix
  const int s = size;
  const double r = ratio;
  const int nP = numProcs;

  //Initialisation
  double nbIter = 1000;
  Matrix M1 = createBigMatrix(s,r);
  CSRMatrix M1CSR(M1);
  Vector V(s);
  for(int i=0; i<s;i++){V[i] = i+1;}

  //Lancement de numProcs calculs
  for(int n = 1; n<=nP; n++){
    ofstream log;
    string n_str;//string which will contain the result
    stringstream convert; // stringstream used for the conversion
    convert << n;//add the value of Number to the characters in the stream
    n_str = convert.str();//s
    string name;
    if (n<10){name = "computation_times_par_00" + n_str + ".txt";}
    else if ((n>9) && (n<100)){name = "computation_times_par_0" + n_str + ".txt";}
    else if (n>99){name = "computation_times_par_" + n_str + ".txt";}

    log.open(name.c_str());
    log << "Temps de calcul pour une matrice " << s << " x " << s << " avec un ratio d'éléments non nuls de " << r << endl;
    for (int i = 0 ; i < nbIter ; i++) {
      double start_time = omp_get_wtime();
      Vector result_vector = parMult(M1CSR, V, n);
      double final_time = omp_get_wtime() - start_time;
      log << final_time << endl;
    }
    log.close();
  }
}

void launchParallelCSRProductOnData(int nP){
  //Initialisation
  double nbIter = 10;
  CSRMatrix M("newMatrix.data");
  Vector V("newVector.data");
  cout << "Ouverture des fichiers terminée" << endl;

  //Lancement de nP calculs
  for(int n = 1; n<=nP; n++){
    //Création des flux
    ofstream log;
    string n_str;
    stringstream convert;
    convert << n;
    n_str = convert.str();

    //Initialisation du nom du fichier résultat en fonction du nombre de coeurs
    string name;
    if (n<10){name = "computation_times_par_00" + n_str + ".txt";}
    else if ((n>9) && (n<100)){name = "computation_times_par_0" + n_str + ".txt";}
    else if (n>99){name = "computation_times_par_" + n_str + ".txt";}

    //Ouverture du fichier et écriture du header
    log.open(name.c_str());
    log << M.GetNumberOfRows() << " " << M.GetNNZ() << " " << nP << endl;
    for (int i = 0 ; i < nbIter ; i++) {
      Vector result_vector(M.GetNumberOfRows());
      double start_time = omp_get_wtime();
      if(n==1){result_vector = M*V;}
      else{result_vector = parMult(M, V, n);}
      double final_time = omp_get_wtime() - start_time;
      log << final_time << endl;
    }
    //Fermeture du flux en écriture
    log.close();
  }
}

Vector getResult(){
  CSRMatrix M("newMatrix.data");
  Vector V("newVector.data");
  Vector X = parMult(M,V,4);
  //Vector X = M*V;
  return X;
}

