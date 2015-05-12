#!/bin/bash
#

#Teste que le premier argument soit "openmp" ou "hybrid"
if [ "$1" != "openmp" ] && [ "$1" != "hybrid" ]
then
    echo "Error: First argument must be \"openmp\" or \"hybrid\""
    exit
fi

#Si le premier argument est openmp, on attend un seul argument en plus
#On vérifie que le second argument soit un entier convenable
if [ "$1" = "openmp" ]
then
    if [ "$#" != 2 ]
    then
	echo -e "Error: Needs a second argument: number of threads"
	exit
    else
	if (("$2"<=0)) || (("$2">512))
	then
	    echo "Error: Second argument for openmp must be an integer in range 0 - 512 "
	    exit
	fi
    fi
fi


#Si le premier argument est hybrid, on attend deux arguments en plus
#On vérifie que les deux autres arguments soient des entiers convenables
if [ "$1" = "hybrid" ]
then
    if [ "$#" != 3 ]
    then
	echo -e "Error: Needs two other arguments:\n\tNumber of MPI processes\n\tNumber of threads per MPI process"
	exit
    else
	if (("$2"<=0)) || (("$2">512))
	then
	    echo "Error: Number of MPI processes must be an integer in range 0 - 128"
	    exit
	fi
	if (("$3"<=0)) || (("$3">512))
	then
	    echo "Error: Number of threads must be an integer in range 0 - 512"
	    exit
	fi
    fi
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
    qsub -v nP=$2 -l select=1:ncpus=$2 scriptOpenMP.sh
fi

#Hybride
if [ "$1" = "hybrid" ]
then
    #module load intel-mpi/5.0.1.035
    module load mpt/2.11
    make main -f MakefileHybrid
    qsub -v nMPI=$2,nOMP=$3 -l select=1:ncpus=$(( $2*$3 )) scriptHybrid.sh
fi
