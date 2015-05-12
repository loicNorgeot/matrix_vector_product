#!/bin/bash
#
#PBS -l select=1:ncpus=1
#PBS -l walltime=05:00:00
#PBS -N matrix_creation
#PBS -j oe 

cp $PBS_O_WORKDIR/makeMatrix.py .
cp $PBS_O_WORKDIR/csr.py .
python makeMatrix.py > log.txt
cp *.txt $PBS_O_WORKDIR

