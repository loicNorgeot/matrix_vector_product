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
      if(M.mData[i][j]!=0){//if elt!=0
	A[nnz] = M.mData[i][j];//elt storingg
	JA[nnz] = j;//col index storing
	if (firstElt){//if first of the line
	  IA[i] = nnz;//line index storing
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
  //variables recording
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
CSRMatrix::CSRMatrix(std::string fileName){
  mA = NULL;
  mIA = NULL;
  mJA = NULL;
  
  /*
  // ----------------
  //Méthode récente
  // ----------------  
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;
  while (std::getline(file, str)){
    //On charge mNNZ et mA sur la ligne n°1, après le header
    if(line == 1){
      std::vector<double> vect = split<double>(str);
      mNNZ = vect.size();
      mA = new double[mNNZ];
      for(int i=0;i<mNNZ;i++){
        mA[i] = vect[i];
      }
    } 
    //On charge mNumRows et mIA sur la ligne n°2
    if(line == 2){
      std::vector<int> vect = split<int>(str);
      mNumRows = vect.size() - 1;
      mIA = new int[mNumRows + 1];
      for(int i=0;i<mNumRows + 1;i++){
        mIA[i] = vect[i];
      }
    } 
    //On charge mJA sur la dernière ligne
    if(line == 3){
      std::vector<int> vect = split<int>(str);
      mJA = new int[mNNZ];
      for(int i=0;i<mNNZ;i++){
        mJA[i] = vect[i];
      }
    } 
    //On incrémente le numéro de la ligne
    line++;
  }
  */

  /*
  // -------------------
  //Méthode de Pascal
  // -------------------
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;

  int IAbegin = 0;
  int JAbegin = 0;
  int Abegin = 0;
  while (std::getline(file, str)){
    // 1 - Initialisation du hachage grace au header du fichier
    if (line == 0){
      vector<int> vect = split<int>(str);
      //Initialisation des valeurs et tableaux
      mNNZ = vect[2];
      mNumRows = vect[0];
      mA = new double[mNNZ];
      mIA = new int[mNumRows + 1];
      mJA = new int[mNNZ];
      //Initialisation des numéros de ligne pour le hachage
      IAbegin = 3;
      JAbegin = IAbegin + 1 + mNumRows/10;
      if(mNNZ%10==0){Abegin = JAbegin + mNNZ/10;}
      else{Abegin = JAbegin + 1 + mNNZ/10;}
    }
    // 2 - On charge les éléments de IA
    if( (line >= IAbegin) && (line < JAbegin) ){
      int I = line-IAbegin;
      std::vector<int> vect = split<int>(str);
      for(int j=0;j<vect.size();j++){
        mIA[I*10+j] = vect[j];
      }
    }
    // 3 - On charge les éléments de JA
    if( (line >= JAbegin) && (line < Abegin) ){
      int I = line-JAbegin;
      std::vector<int> vect = split<int>(str);
      for(int j=0;j<vect.size();j++){
        mJA[I*10+j] = vect[j];
      }
    }
    // 4 - On charge les éléments de A
    if(line >= Abegin){
      int I = line-Abegin;
      std::vector<int> vect = split<int>(str);
      for(int j=0;j<vect.size();j++){
        mA[I*10+j] = vect[j];
      }
    }
    //On incrémente le numéro de la ligne
    line++;
  }
  */


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

  //IA
  mIA = new int[mNumRows + 1];
  iaFile=fopen((root + "_IA.bin").c_str(),"rb");
  if (!iaFile){
    printf("Unable to open file!");
  }
  fread(mIA,sizeof(*mIA),mNumRows + 1,iaFile);
  fclose(iaFile);

  //JA
  mJA = new int[mNNZ];
  jaFile=fopen((root + "_JA.bin").c_str(),"rb");
  if (!jaFile){
    printf("Unable to open file!");
  }
  fread(mJA,sizeof(*mJA),mNNZ,jaFile);
  fclose(jaFile);

  //A
  mA = new double[mNNZ];
  aFile=fopen((root + "_A.bin").c_str(),"rb");
  if (!aFile){
    printf("Unable to open file!");
  }
  fread(mA,sizeof(*mA),mNNZ,aFile);
  fclose(aFile);
}

//Destructor
CSRMatrix::~CSRMatrix(){
  delete[] mA;
  delete[] mIA;
  delete[] mJA;
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
    double t0=omp_get_wtime(), t1=0,t2=0,t3=0;

    //V_copy initialization
    double *v_copy=NULL;
    v_copy = new double[nR];
#pragma omp for schedule(auto)
    for(int i=0; i<nR; i++){
      v_copy[i] = v.Read(i);
    }
    if(id==0){t1 = omp_get_wtime(); log << "1: " << t1-t0 << endl;}

    //Product computation
#pragma omp for schedule(auto)
    for(int i=0; i<nR; i++){
      double s_temp = 0.0;
      for(int j=m.mIA[i]; j<m.mIA[i+1]; j++){
	s_temp += m.mA[j]*v_copy[m.mJA[j]];
      }
      sol[i] = s_temp;
    }

    delete[] v_copy;
    if(id==0){t2 = omp_get_wtime(); log << "2: " << t2-t1 << endl;}
  }
  log.close();

  return sol;
}





