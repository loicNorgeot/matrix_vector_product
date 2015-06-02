#include "omp.h"
#include <iostream>
#include <string>

using namespace std;

double chrono(double t0, int& compteur, string message){
  double gapTime = omp_get_wtime() - t0;
  cout << "\t" << compteur << ":\tt = " << gapTime << ", \t" << message << endl;
  compteur++;
  return omp_get_wtime();
}
