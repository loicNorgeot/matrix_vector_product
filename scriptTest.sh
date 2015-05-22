#!/bin/bash
#
#PBS -l walltime=04:00:00
#PBS -N main
#PBS -j oe 

#Chargement des modules
. /usr/share/modules/init/sh
module load mpt/2.11
module load intel-compilers-15/15.0.0.090

cp $PBS_O_WORKDIR/main .

export SIZE=$size
export KMP_AFFINITY=disabled

/usr/bin/time -v omplace -nt $nP ./main $nP > log.txt

cp *.txt $PBS_O_WORKDIR
