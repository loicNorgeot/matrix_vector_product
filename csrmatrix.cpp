#include "matrix.h"
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
std::vector<T> split(const std::string& line) {
  std::istringstream is(line);
  return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
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

//Matrix class constructor
CSRMatrix::CSRMatrix(const Matrix& M){
  mNumRows = M.GetNumberOfRows();
  int mNumCols = M.GetNumberOfCols();
  int nnz = 0;
  int size_inc = 1500;
  double *A = new double[size_inc];
  int IA[mNumRows+1];
  int *JA = new int[size_inc];
  
  for(int i=0;i<mNumRows;i++){
    bool firstElt = true;
    for(int j=0;j<mNumCols;j++){
      if(M.mData[i][j]!=0){
	A[nnz] = M.mData[i][j];
	JA[nnz] = j;
	if (firstElt){
	  IA[i] = nnz;
	  firstElt = false;
	}
	nnz++;
      }
      if(j==mNumCols-1){
	if(firstElt){
	  IA[i]=0;
	}
      }
    }
    //A array size modification
    double *tA = new double[nnz+size_inc];
    for (int k=0;k<nnz;k++){tA[k]=A[k];}
    delete[] A;
    A = tA;
    //JA array size modification
    int *tJA = new int[nnz+size_inc];
    for (int k=0;k<nnz;k++){tJA[k]=JA[k];}
    delete[] JA;
    JA = tJA;
  }

  IA[mNumRows] = nnz;
  mNNZ = nnz;
  mA = new double[mNNZ];
  mJA = new int[mNNZ];
  mIA = new int[mNumRows+1];
  for(int i = 0; i<mNNZ; i++){
    mA[i] = A[i];
    mJA[i] = JA[i];}
  for(int i =0; i<mNumRows+1; i++){
    mIA[i] = IA[i];
  }
}

//File constructor
CSRMatrix::CSRMatrix(std::string fileName, const int nbProcs){
  mA = NULL;
  mIA = NULL;
  mJA = NULL;
  
  // ----------------------------------------
  //       Method from binary files
  // ----------------------------------------
  FILE *aFile, *iaFile, *jaFile;
  string dataPath = "/work/norgeot/";
  string varName = "SIZE";
  string SIZE(std::getenv(varName.c_str()));
  string root = dataPath + "matrix_" + SIZE;

  //Header reading
  ifstream infile((root + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    mNumRows = line[0];
    mNNZ = line[1];
    cout << mNNZ << " " << mNumRows << endl;
  }

  //Binary files reading
  int *mIAt=NULL;
  mIAt = new int[mNumRows+1];
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
  mIA = new int[mNumRows + 1];
  mJA = new int[mNNZ];
  mA = new double[mNNZ];

#pragma omp parallel for num_threads(nbProcs) schedule(static,mNumRows/nbProcs)
  for (int i = 0 ; i < mNumRows; i++ ){
    mIA[i] = mIAt[i];
    mIA[i+1] = mIAt[i+1];
    for(int j=mIA[i]; j<mIA[i+1]; j++){
      mA[j] = mAt[j];
      mJA[j] = mJAt[j];
    }
  }
  delete[] mIAt;
  delete[] mJAt;
  delete[] mAt;

  /*
  // -------------------------------
  //        ESSAI DE PREAD
  // -------------------------------
  FILE *aFile, *iaFile, *jaFile;
  string dataPath = "/work/norgeot/";
  string varName = "SIZE";
  string SIZE(std::getenv(varName.c_str()));
  string root = dataPath + "matrix_" + SIZE;
  //Header reading
  ifstream infile((root + "_H.data").c_str());
  string str;
  while(getline(infile, str)){
    vector<int> line = split<int>(str);
    mNumRows = line[0];
    mNNZ = line[1];
    cout << mNNZ << " " << mNumRows << endl;
  }
  iaFile=fopen((root + "_IA.bin").c_str(),"rb");
  aFile=fopen((root + "_A.bin").c_str(),"rb");
  jaFile=fopen((root + "_JA.bin").c_str(),"rb");
  
  const int chunk_size = mNumRows/nbProcs;

  mIA = new int[mNumRows + 1];
#pragma omp parallel num_threads(nbProcs)
  {
    int offset = omp_get_thread_num() * chunk_size;
    int *mIAt = NULL;
    mIAt = new int[chunk_size];    
    int offset_IA = offset * sizeof(*mIAt);

    //pread(iaFile, mIAt, sizeof(*mIAt), offset_IA);
    fseek(iaFile,offset_IA,SEEK_SET);
    fread(mIAt, sizeof(*mIAt), chunk_size, iaFile);

#pragma omp for schedule(static,mNumRows/nbProcs)
    for (int i = 0 ; i < mNumRows; i++ ){
      int k = i%chunk_size;
      mIA[i] = mIAt[k];
    }
    delete[] mIAt;
  }

  mJA = new int[mNNZ];
  mA = new double[mNNZ];
#pragma omp parallel num_threads(nbProcs)
  {
    int lowerInd = omp_get_thread_num()*chunk_size;
    int upperInd = (omp_get_thread_num()+1) * chunk_size;
    const int diff = mIA[upperInd] - mIA[lowerInd];

    int *mJAt = NULL;
    mJAt = new int[diff];
    //pread(jaFile, mJAt, sizeof(*mJAt), mIA[lowerInd]);
    fseek(jaFile,mIA[lowerInd],SEEK_SET);
    cout << omp_get_thread_num() << " ";
    fread(mJAt, sizeof(*mJAt), diff, jaFile);

    double *mAt = NULL;
    mAt = new double[diff];
    cout << omp_get_thread_num() << endl;
    //pread(aFile, mAt, sizeof(*mAt), mIA[lowerInd]);
    fseek(aFile,mIA[lowerInd],SEEK_SET);
    fread(mAt, sizeof(*mAt), diff, aFile);
    

#pragma omp for schedule(static,mNumRows/nbProcs)
    for (int i = 0 ; i < mNumRows; i++ ){
      for (int j = mIA[i] ; j < mIA[i+1] ; i++){
	mJA[j] = mJAt[j-lowerInd];
	mA[j] = mAt[j - lowerInd];
      }
    }
    delete[] mAt;
    delete[] mJAt;
  }
  
  fclose(jaFile);
  fclose(aFile);
  fclose(iaFile);
  */
}


//Destructor
CSRMatrix::~CSRMatrix(){
  delete[] mA;
  delete[] mIA;
  delete[] mJA;
  //delete[] mX;
}

//Private variable accessing
int CSRMatrix::GetNNZ() const{return mNNZ;}
int CSRMatrix::GetNumberOfRows() const{return mNumRows;}
double CSRMatrix::GetA(int i) const{return mA[i];}
int CSRMatrix::GetIA(int i) const{return mIA[i];}
int CSRMatrix::GetJA(int i) const{return mJA[i];}

//Print
std::ostream& operator<<(std::ostream& output, const CSRMatrix& m){
  //Printing A and JA
  if(m.mNNZ<10){//less than 10 elements
    cout << "\nA = ";
    for(int i =0; i<m.mNNZ;i++){cout<<m.mA[i]<<"; ";}
    cout << "\nJA = ";
    for(int i =0; i<m.mNNZ;i++){cout<<m.mJA[i]<<"; ";}
  }
  else{//more than 10 elements
    cout << "\nA = ";
    cout<<m.mA[0]<<"; "<<m.mA[1]<<" ... "<<m.mA[m.mNNZ-2]<<"; "<<m.mA[m.mNNZ-1];
    cout << "\nJA = ";
    cout<<m.mJA[0]<<"; "<<m.mJA[1]<<" ... "<<m.mJA[m.mNNZ-2]<<"; "<<m.mJA[m.mNNZ-1];
  }
  //IA
  if(m.mNumRows<10){//less than 10 elements
    cout << "\nIA = ";
    for(int i =0; i<m.mNumRows;i++){cout<<m.mIA[i]<<"; ";}
    cout << "/ " << m.mIA[m.mNumRows];
  }
  else{//more than 10 elements
    cout << "\nIA = ";
    cout<<m.mIA[0]<<"; "<<m.mIA[1]<<" ... "<<m.mIA[m.mNumRows-2]<<"; "<<m.mIA[m.mNumRows-1];
    cout << "/ " << m.mIA[m.mNumRows];
  } 
}
  
//Sequential multiplication
Vector operator*(const CSRMatrix& m, const Vector& v){
  Vector sol(m.mNumRows);
  for(int i=0; i<m.mNumRows; i++){
    double s_temp = 0.0;
    for(int j=m.mIA[i]; j<m.mIA[i+1]; j++){
      s_temp += m.mA[j]*v.Read(m.mJA[j]);
      //s_temp+= m.mX[2*j]*v.Read(m.mX[2*j+1]);
    }
    sol[i] = s_temp;
  }
  return sol;
}

//Parallel multiplication
Vector parMult(const CSRMatrix& m, const Vector& v, const int nbProcs){
  const int nR = m.GetNumberOfRows();
  const int nnz = m.GetNNZ();
  Vector sol(nR);

  string n_str;
  stringstream convert;
  convert << nbProcs;
  n_str = convert.str();
  string name = "times" + n_str + ".txt";
  ofstream log;
  log.open(name.c_str());

  //Parallel part
#pragma omp parallel num_threads(nbProcs)
  {
    int id = omp_get_thread_num();
    double t0=omp_get_wtime(), t1=0, t2=0, t3=0, t4=0;
    
    //V_copy initialization
    double *v_copy=NULL;
    v_copy = new double[nR];
#pragma omp for schedule(static, nR/nbProcs)
    for(int i=0; i<nR; i++){
      v_copy[i] = v.Read(i);
    }
    t1 = omp_get_wtime();
    if(id==0){log << "1: " << t1-t0 << endl;}
    
    //Product computation
#pragma omp for schedule(static, nR/nbProcs)
    for(int i=0; i<nR; i++){
      double s_temp = 0.0;
      for(int j=m.mIA[i]; j<m.mIA[i+1]; j++){
	s_temp += m.mA[j]*v_copy[m.mJA[j]];
      }
      sol[i] = s_temp;
    }
    t2 = omp_get_wtime();
    if(id==0){log << "2: " << t2-t1 << endl;}

    delete[] v_copy;
    t3 = omp_get_wtime();
    if(id==0){log << "3: " << t3-t2 << endl;}
  }
  log.close();
  return sol;
}





