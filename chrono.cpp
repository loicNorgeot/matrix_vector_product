#include <iostream>
#include <string>

#include "omp.h"

using namespace std;

//Time function, taking an old time and an increment
double chrono(double t0, int& compteur, string message){
  double gapTime = omp_get_wtime() - t0;
  cout << "\t" << compteur << ":\tt = " << gapTime << ", \t" << message << endl;
  compteur++;
  return omp_get_wtime();
}
