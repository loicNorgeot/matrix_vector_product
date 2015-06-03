#include <cmath>
#include <iostream>
#include <cassert>
#include <string>

#include "csrmatrix.h"
#include "vector.h"
#include "binaryIO.h"

#include "omp.h"

using namespace std;


///////////////////////////////////////////
//CONSTRUCTORS & DESTRUCTOR

//File
CSRMatrix::CSRMatrix(string inPath,
		     string name,
		     const int nbProcs){
  //Variables
  int nR=0;
  unsigned int nnz=0;
  getHeaderInfo(nR, nnz, inPath, name);
  mNumRows = nR;
  mNNZ = nnz;
  //Temporary arrays initialization
  unsigned int *IAt = new unsigned int[nR + 1];
  int *JAt = new int[nnz];
  double *At = new double[nnz];
  //Binary Reading
  brMatrix(IAt, JAt, At, nR, nnz, inPath, name);
  //Final arrays initialization
  mA = NULL;
  mIA = NULL;
  mJA = NULL;
  mIA = new unsigned int[mNumRows + 1];
  mJA = new int[mNNZ];
  mA = new double[mNNZ];
  //First touch
#pragma omp parallel for num_threads(nbProcs) schedule(static,mNumRows/nbProcs)
  for (int i = 0 ; i < mNumRows; i++ ){
    mIA[i] = IAt[i];
    mIA[i+1] = IAt[i+1];
    for(unsigned int j=mIA[i]; j<mIA[i+1]; j++){
      mA[j] = At[j];
      mJA[j] = JAt[j];
    }
  }
  //Unallocation
  delete[] IAt;
  delete[] JAt;
  delete[] At;
}

//Destructor
CSRMatrix::~CSRMatrix(){
  delete[] mA;
  delete[] mIA;
  delete[] mJA;
}


///////////////////////////////////////////////////
//PRIVATE VARIABLES ACCESSING

//Number of Non Zeros elements
unsigned int CSRMatrix::GetNNZ() const{return mNNZ;}

//Number of Rows
int CSRMatrix::GetNumberOfRows() const{return mNumRows;}

//Arrays IA, JA and A
unsigned int CSRMatrix::GetIA(int i) const{return mIA[i];}
int CSRMatrix::GetJA(int i) const{return mJA[i];}
double CSRMatrix::GetA(int i) const{return mA[i];}
  

///////////////////////////////////////////////////
//MATRIX VECTOR PRODUCT

//Sequential
Vector CSRMatrix::operator*(const Vector& v) const{
  assert(v.GetSize()==mNumRows);
  Vector sol(mNumRows);
  for(int i=0; i<mNumRows; i++){
    double s_temp = 0.0;
    for(unsigned int j=mIA[i]; j<mIA[i+1]; j++){
      s_temp += mA[j]*v.Read(mJA[j]);
    }
    sol[i] = s_temp;
  }
  return sol;
}

//Parallel
Vector CSRMatrix::parallelProduct(const Vector& v, 
				  const int nbProcs) const{
  assert(v.GetSize()==mNumRows);
  //Variables
  const int nR = mNumRows;
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
    for(unsigned int j=mIA[i]; j<mIA[i+1]; j++){
      s_temp += mA[j]*v_copy[mJA[j]];
    }
    sol[i] = s_temp;
  }
  //Cleaning and return
  delete[] v_copy;
  return sol;
}


///////////////////////////////////////////////////
//OTHER METHODS

//Diagonalization
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
  cout << "Non zeros elements on the diagonal = " << c << endl;
  return D;
}
