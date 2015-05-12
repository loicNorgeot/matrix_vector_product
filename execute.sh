#!/bin/bash
#Scripts d'éxécution

#Nettoyage
rm *.txt
rm main.o*
rm *#
rm *~ 
rm main
rm *.o
rm res.data

#Chargement des modules
module load mpt/2.11
module load intel-compilers-15/15.0.0.090

#Compilation
make main

#Lancement sur mesu
qsub script.sh
