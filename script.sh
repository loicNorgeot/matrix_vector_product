#!/bin/bash
#
#PBS -l select=1:ncpus=32
#PBS -l walltime=00:30:00
#PBS -N main
#PBS -j oe 

#Chargement des modules
. /usr/share/modules/init/sh
module load mpt/2.11
#Pour openMP
module load intel-compilers-15/15.0.0.090
#Pour Hybride
#module load intel-mpi/5.0.1.035

cp $PBS_O_WORKDIR/main .

#Pour OpenMP
omplace -nt 32 ./main 32 > log.txt
#Pour Hybride
#mpirun -np 4 omplace -nt 8 ./main > log32.txt

cp *.txt $PBS_O_WORKDIR
cp *.data $PBS_O_WORKDIR
