#include "csrmatrix.h"
#include "vector.h"

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

//Copy constructor
CSRMatrix::CSRMatrix(const CSRMatrix& otherMatrix){
  mNNZ = otherMatrix.mNNZ;
  mNumRows = otherMatrix.GetNumberOfRows();
  mA = new double[mNNZ];
  mIA = new int[mNumRows+1];
  mJA = new int[mNNZ];
  for(int i = 0; i<mNNZ; i++){
    mA[i] = otherMatrix.mA[i];
    mJA[i] = otherMatrix.mJA[i];}
  for(int i =0; i<mNumRows+1; i++){
    mIA[i] = otherMatrix.mIA[i];
  }
}

//File constructor
CSRMatrix::CSRMatrix(string root, 
		     const int nbProcs){
  mA = NULL;
  mIA = NULL;
  mJA = NULL;
  
  FILE *aFile, *iaFile, *jaFile;

  //Header reading
  ifstream infile((root + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<unsigned int> line = split<unsigned int>(str);
    mNumRows = line[0];
    mNNZ = line[1];
  }

  //Binary files reading
  unsigned int *mIAt=NULL;
  mIAt = new unsigned int[mNumRows+1];
  iaFile=fopen((root + "_IA.bin").c_str(),"rb");
  if (!iaFile){
    printf("Unable to open file!");
  }
  fread(mIAt,sizeof(*mIAt),mNumRows + 1,iaFile);
  fclose(iaFile);

  int *mJAt=NULL;
  mJAt = new int[mNNZ];
  jaFile=fopen((root + "_JA.bin").c_str(),"rb");
  if (!jaFile){
    printf("Unable to open file!");
  }
  fread(mJAt,sizeof(*mJAt),mNNZ,jaFile);
  fclose(jaFile);

  double *mAt=NULL;
  mAt = new double[mNNZ];
  aFile=fopen((root + "_A.bin").c_str(),"rb");
  if (!aFile){
    printf("Unable to open file!");
  }
  fread(mAt,sizeof(*mAt),mNNZ,aFile);
  fclose(aFile);

  //First touch initialization
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
