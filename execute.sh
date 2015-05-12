#!/bin/bash
#

#Teste le nombre d'arguments
if [ "$#" != 2 ]
then
    echo -e "Error: Needs two arguments:\n\tArgument 1 : \"openmp\" or \"hybrid\"\n\tArgument 2 : Number of cores to use"
    exit
fi

#Teste que le premier argument soit "openmp" ou "hybrid"
if [ "$1" != "openmp" ] && [ "$1" != "hybrid" ]
then
    echo "Error: First argument must be \"openmp\" or \"hybrid\""
    exit
fi

#Teste que le second argument soit compris entre 0 et 512
if (("$2"<=0)) || (("$2">512))
then
    echo "Error: Second argument must be an integer in range 0 - 512 "
    exit
fi

#Nettoyage
rm *.txt
rm main.o*
rm *#
rm *~ 
rm main
rm *.o
rm res.data

#OpenMP
if [ "$1" = "openmp" ]
then
    module load mpt/2.11
    module load intel-compilers-15/15.0.0.090
    make main -f MakefileOpenMP
    qsub scriptOpenMP.sh
fi

#Hybride
if [ "$1" = "hybrid" ]
then
    #module load intel-mpi/5.0.1.035
    module load mpt/2.11
    make main -f MakefileHybrid
    qsub scriptHybrid.sh
fi
