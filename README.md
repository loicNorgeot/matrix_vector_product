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


Commandes utiles pour le suivi des jobs, après ssh uv:

	  time -p dans les scripts de lancement, au tout début de la ligne de commande (avant omplace)

	  qstat -a : permet de récupérer la SESSIONID
	  
	  wts SESSIONID -L : permet d'avoir, pour le job lancé, une liste des LWP (threads), pourcentages de CPU, numéros de processeurs...

	  pmgcluster -t 10 : permet de suivre l'utilisation de MESU

	  perf top -p LWP : permet de connaitre, pour un thread donné, les fonctions utilisées

	  -vec-report=5 : flag de reporting pour les compilateurs intel, qui permet d'avoir un monitoring de la vectorisation

	  nodeinfo : Permet de savoir si les accès sont locaux ou distants

	  top -u USERID : top sur les process de l'utilisateur

	  

	  