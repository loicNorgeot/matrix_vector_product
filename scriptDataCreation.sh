#!/bin/bash
#
#PBS -l select=1:ncpus=64
#PBS -l walltime=04:00:00
#PBS -N matrix_creation
#PBS -j oe 

#Vérification de la validité de l'argument
if (("$nnz"<10000)) || (("$nnz">10000000000))
then
    echo "Error: NNZ must be in range 1e4 - 1e10 "
    exit
fi

cp $PBS_O_WORKDIR/makeMatrix.py .
cp $PBS_O_WORKDIR/csr.py .
python makeMatrix.py $nnz
cp *.txt $PBS_O_WORKDIR

