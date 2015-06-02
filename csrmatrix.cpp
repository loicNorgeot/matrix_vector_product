#include "csrmatrix.h"
#include "vector.h"
#include "binaryIO.h"

#include "omp.h"

#include <string>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cstdlib>

using namespace std;

template<typename T>
vector<T> split(const string& line) {
  istringstream is(line);
  return vector<T>(istream_iterator<T>(is), istream_iterator<T>());
}


//File constructor
CSRMatrix::CSRMatrix(string root, 
		     const int nbProcs){
  //Variables
  int nR=0, nnz=0;
  getHeaderInfo(nR, nnz, "test");
  mNumRows = nR;
  mNNZ = nnz;
  
  //Initialisation
  int *IAt = new int[nR + 1];
  int *JAt = new int[nnz];
  double *At = new double[nnz];

  //Lecture binaire
  brMatrix(IAt, JAt, At, nR, nnz, "test");

  //First touch initialization
  mA = NULL;
  mIA = NULL;
  mJA = NULL;
  mIA = new unsigned int[mNumRows + 1];
  mJA = new int[mNNZ];
  mA = new double[mNNZ];

#pragma omp parallel for num_threads(nbProcs) schedule(static,mNumRows/nbProcs)
  for (int i = 0 ; i < mNumRows; i++ ){
    mIA[i] = mIAt[i];
    mIA[i+1] = mIAt[i+1];
    for(unsigned int j=mIA[i]; j<mIA[i+1]; j++){
      mA[j] = mAt[j];
      mJA[j] = mJAt[j];
    }
  }
  delete[] mIAt;
  delete[] mJAt;
  delete[] mAt;
}

//Destructor
CSRMatrix::~CSRMatrix(){
  delete[] mA;
  delete[] mIA;
  delete[] mJA;
}

//Private variable accessing
unsigned int CSRMatrix::GetNNZ() const{return mNNZ;}
int CSRMatrix::GetNumberOfRows() const{return mNumRows;}

//Diagonalisation
Vector CSRMatrix::toDiagonal() const{
  Vector D(mNumRows);
  double c = 0;
  for(int i = 0 ; i < mNumRows ; i++){
    for(unsigned int j = mIA[i] ; j < mIA[i+1] ; j++){
      if(mJA[j] == i){
	D[i] = mA[j];
	c++;
      }
    }
  }
  cout << "Nombre d'éléments non nuls sur la diagonale = " << c << endl;
  return D;
}
  
//Sequential multiplication
Vector operator*(const CSRMatrix& m,
		 const Vector& v){
  Vector sol(m.mNumRows);
  for(int i=0; i<m.mNumRows; i++){
    double s_temp = 0.0;
    for(unsigned int j=m.mIA[i]; j<m.mIA[i+1]; j++){
      s_temp += m.mA[j]*v.Read(m.mJA[j]);
    }
    sol[i] = s_temp;
  }
  return sol;
}

//Parallel multiplication
Vector parMult(const CSRMatrix& m, 
	       const Vector& v, 
	       const int nbProcs){

  const int nR = m.GetNumberOfRows();
  const int chunk = nR/nbProcs;
  Vector sol(nR);

  //V_copy initialization
  double *v_copy=NULL;
  v_copy = new double[nR];
  for(int i=0; i<nR; i++){
    v_copy[i] = v.Read(i);
  }

  //Parallel part
#pragma omp parallel for num_threads(nbProcs) firstprivate(v_copy) schedule(static, chunk)
  for(int i=0; i<nR; i++){
    double s_temp = 0.0;
    for(unsigned int j=m.mIA[i]; j<m.mIA[i+1]; j++){
      s_temp += m.mA[j]*v_copy[m.mJA[j]];
    }
    sol[i] = s_temp;
  }
  delete[] v_copy;
  return sol;
}
