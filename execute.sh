#!/bin/bash
#

#Teste que le premier argument soit "openmp" ou "hybrid"
if [ "$1" != "openmp" ] && [ "$1" != "hybrid" ]
then
    echo "Error: First argument must be \"openmp\" or \"hybrid\""
    exit
fi

#Si le premier argument est openmp, on attend deux arguments en plus
#On vérifie que le second argument soit un entier convenable
if [ "$1" = "openmp" ]
then
    if [ "$#" != 3 ]
    then
	echo -e "Error: Needs two other arguments:\n\tSize of the matrix (i.e. 1e+07)\n\tNumber of threads"
	exit
    else
	#RAJOUTER ICI UN TEST DE REGEXP SUR LA TAILLE DE LA MATRICE
	if (("$3"<=0)) || (("$3">512))
	then
	    echo "Error: Third argument for openmp must be an integer in range 0 - 512 "
	    exit
	fi
    fi
fi


#Si le premier argument est hybrid, on attend trois arguments en plus
#On vérifie que les deux autres arguments soient des entiers convenables
if [ "$1" = "hybrid" ]
then
    if [ "$#" != 4 ]
    then
	echo -e "Error: Needs three other arguments:\n\tSize of the matrix (i.e. 1e+07)\n\tNumber of MPI processes\n\tNumber of threads per MPI process"
	exit
    else
	#RAJOUTER ICI UN TEST DE REGEXP SUR LA TAILLE DE LA MATRICE
	if (("$3"<=0)) || (("$3">512))
	then
	    echo "Error: Number of MPI processes must be an integer in range 0 - 128"
	    exit
	fi
	if (("$4"<=0)) || (("$4">512))
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
    qsub -v nP=$3,size=$2 -l select=1:ncpus=$3 scriptOpenMP.sh
fi

#Hybride
if [ "$1" = "hybrid" ]
then
    #module load intel-mpi/5.0.1.035
    module load mpt/2.11
    make main -f MakefileHybrid
    qsub -v nMPI=$3,nOMP=$4,size=$2 -l select=1:ncpus=$(( $3*$4 )) scriptHybrid.sh
fi
