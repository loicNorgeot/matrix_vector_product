#!/bin/bash
#
#PBS -l walltime=00:30:00
#PBS -N main
#PBS -j oe 

#Chargement des modules
. /usr/share/modules/init/sh
module load mpt/2.11
#module load intel-mpi/5.0.1.035

cp $PBS_O_WORKDIR/main .

export SIZE=$size
mpirun -np $nMPI omplace -nt $nOMP ./main > log.txt
#mpiexec_mpt -np $nMPI ./main > log.txt
cp *.txt $PBS_O_WORKDIR
cp *.data $PBS_O_WORKDIR
