#!/bin/bash
#
#PBS -l select=1:ncpus=128
#PBS -l walltime=00:30:00
#PBS -N main
#PBS -j oe 

. /usr/share/modules/init/sh
module load mpt/2.11
#module load intel-mpi/5.0.1.035

cp $PBS_O_WORKDIR/main .
 
mpirun -np 16 omplace -nt 8 ./main > log128.txt

cp res.data $PBS_O_WORKDIR
cp *.txt $PBS_O_WORKDIR
