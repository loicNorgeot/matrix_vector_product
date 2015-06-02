#!/bin/bash
#

#Teste que le premier argument soit "openmp" ou "conjugate"
if [ "$1" != "openmp" ] && [ "$1" != "conjugate" ]
then
    echo "Error: First argument must be \"openmp\" or \"conjugate\""
    exit
fi

#Si le premier argument est openmp OU CONJUGATE, on attend deux arguments en plus
if [ "$#" != 3 ]
then
    echo -e "Error: Needs two other arguments:\n\tSize of the matrix (i.e. 1e+07)\n\tNumber of threads"
    exit
else
    if (("$3"<=0)) || (("$3">512))
    then
	echo "Error: Third argument for openmp must be an integer in range 0 - 512 "
	exit
    fi
fi

#Nettoyage
./clean.sh

module load mpt/2.11
module load intel-compilers-15/15.0.0.090

if [ "$1" = "openmp" ]
then
    make main -f MakefileOpenMP
elif [ "$1" = "conjugate" ]
then
    make main -f MakefileGC
fi

qsub -v nP=$3,size=$2 -l select=1:ncpus=$3 script.sh
