#!/bin/bash
#
#PBS -lselect=1:ncpus=32
#PBS -l walltime=00:30:00
#PBS -N main
#PBS -j oe 

# load modules
. /usr/share/modules/init/sh
module load mpt/2.11
module load intel-compilers-15/15.0.0.090

cp $PBS_O_WORKDIR/main .
#cp $PBS_O_WORKDIR/*6.data .

#export OMP_NUM_THREADS = 32
#./main 32 > log.txt
omplace -nt 32 ./main 32 > log.txt

cp *.txt $PBS_O_WORKDIR
cp Resultat.data $PBS_O_WORKDIR
