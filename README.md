Sur mesu, dans le répertoire de travail:


./execute.sh hybrid matrixSize nMPI nTHREADS

	     matrixSize: de la forme "1e+07", correspond à nnz, présent dans le nom des fichiers .bin et .data

	     nMPI: nombre de process MPI

	     nThreads: nombre de threads OpenMP par process MPI


./execute.sh openmp matrixSize nTHREADS

	     matrixSize: de la forme "1e+07", correspond à nnz, présent dans le nom des fichiers .bin et .data

	     nThreads: nombre de threads OpenMP


qsub -v nnz=10000 scriptDataCreation.sh

     nnz: nombre d'éléments non nuls dans la matrice