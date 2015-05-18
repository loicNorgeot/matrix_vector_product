//Classe vecteur
#include <cmath>
#include <iostream>
#include <cassert>
#include "vector.h"
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

//Constructeur de copie
Vector::Vector(const Vector& otherVector){
  mSize = otherVector.GetSize();
  mData = new double[mSize];
  for(int i=0; i<mSize; i++){
    mData[i] = otherVector.mData[i];
  }
}

//Constructeur en fonction de la taille
Vector::Vector(int size){
  mData = NULL;
  assert(size>0);
  mSize = size;
  mData = new double[mSize];
  for (int i = 0; i<mSize; i++){
    mData[i] = 0.0;
  }
}

//Constructeur à partir d'un fichier
Vector::Vector(std::string fileName) {
  mData = NULL;
  //const std::string dataPath = "/work/norgeot/";

  /*
  //Méthode sur une ligne
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;
  while (std::getline(file, str)){
    std::vector<double> vect = split<double>(str);
    if(line == 1){
      mSize = vect.size();
      mData = new double[mSize];
      for(int i=0;i<mSize;i++){
        mData[i] = vect[i];
      }
    } 
    line++;
  }
  */

  /*
  //Méthode sur plusieurs lignes
  std::ifstream file(fileName.c_str());
  std::string str; 
  int line = 0;
  while (std::getline(file, str)){
    if(line==0){
      std::vector<int> vect = split<int>(str);
      mSize = vect[0];
      mData = new double[mSize];
    }
    else{
      std::vector<double> vect = split<double>(str);
      mData[line-1] = vect[0];
    }
    line++;
  }
  */


  //Méthode binaire
  //Création des noms de fichiers
  string dataPath = "/work/norgeot/";
  string varName = "SIZE";
  string SIZE(std::getenv(varName.c_str()));
  string root = dataPath + "vector_" + SIZE;

  //Lecture du header
  std::string str;
  std::ifstream infile((root + "_H.data").c_str());
  while(std::getline(infile, str)){
    std::vector<int> line = split<int>(str);
    mSize = line[0];}

  //Lecture du vecteur
  FILE *vFile;
  mData = new double[mSize];
  vFile=fopen((root + "_V.bin").c_str(),"rb");
  if (!vFile){
    printf("Unable to open file!");
  }
  fread(mData,sizeof(*mData),mSize,vFile);
  fclose(vFile);
}

//Destructeur
Vector::~Vector(){
  delete[] mData;
}

//Methode pour récupérer la taille
int Vector::GetSize() const{
  return mSize;
}

//Surcharge des [] pour l'indexation
double& Vector::operator[](int i){
  assert(i>-1);
  assert(i < mSize);
  return mData[i];
}

//Variante read only de l'opérateur bracket
double Vector::Read(int i) const
{
  assert(i > -1);
  assert(i < mSize);
  return mData[i];
}

//Surcharge des parenthèses pour un indexage à partir de 1:
double& Vector::operator()(int i){
  assert(i>0);
  assert(i<mSize+1);
  return mData[i-1];
}

//Surcharge de l'opérateur =
Vector& Vector::operator=(const Vector& otherVector){
  assert(mSize == otherVector.mSize);
  for(int i=0;i<mSize;i++){
    mData[i] = otherVector.mData[i];
  }
  return *this;
}

//Surcharge de l'addition unitaire
Vector Vector::operator+() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = mData[i];
  }
  return v;
}
//Surcharge de la soustraction unitaire
Vector Vector::operator-() const{
  Vector v(mSize);
  for(int i=0; i<mSize;i++){
    v[i] = -mData[i];
  }
  return v;
}

//Surcharge de l'addition binaire
Vector Vector::operator+(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] + v1.mData[i];
  }
  return v;
}
//Surcharge de la soustraction binaire
Vector Vector::operator-(const Vector& v1) const{
  assert(mSize == v1.mSize);
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i] = mData[i] - v1.mData[i];
  }
  return v;
}

//Surcharge de la multiplication par un scalaire
Vector Vector::operator*(double a) const{
  Vector v(mSize);
  for(int i=0;i<mSize;i++){
    v[i]=a*mData[i];
  }
  return v;
}

// Calcul de la norme euclidienne, p=2 par défaut
double Vector::CalculateNorm(int p) const{
  double norm_val, sum = 0.0;
  for (int i=0; i<mSize; i++){
    sum += pow(fabs(mData[i]), p);
  }
  norm_val = pow(sum, 1.0/((double)(p)));
  return norm_val;
}

//Retour de la taille avec length
int length(const Vector& v){
  return v.mSize;
}

//Print du vecteur en foncion de la taille
std::ostream& operator<<(std::ostream& output, const Vector& v){
  std::string sep = " ";
  output << "(";
  if (v.mSize<10){
    for (int i=0; i<v.mSize;i++){
      output << v.mData[i] << sep;
    }
  }
  else{
    output << v.mData[0] << sep << v.mData[1] << " ... " << v.mData[v.mSize-2] << sep << v.mData[v.mSize - 1];
  }
  output << ")" << std::endl;
}
