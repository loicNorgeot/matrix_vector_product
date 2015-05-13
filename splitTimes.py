import numpy as np
import matplotlib.pyplot as plt
import os

#Liste des fichiers
FILES = os.listdir("../codes")
FILES = [f for f in FILES if "times" in f and "computation" not in f]

#Liste des nombres de processeurs
PROCS = [f[5:] for f in FILES]
PROCS = [int(p[:-4]) for p in PROCS]

#Obtention des temps
L = [open(f,'r').readlines() for f in FILES]
T = np.array([[float(l.split()[1]) for l in lines] for lines in L])

#Tri par ordre de temps
X = [(p,t) for p,t in zip(PROCS,T)]
X.sort()

p = [x[0] for x in X]
t = [[x[1][i] for x in X] for i in range(len(X[0][1]))]
n = ["Initialisation de sol_temp",
     "Overhead de l'ouverture des threads",
     "Copie privee du vecteur",
     "Produit parallele",
     "Desallocation de la copie du vecteur",
     "Fermeture de la section parallele",
     "Creation du vecteur solution a partir de sol_temp",
     "Desallocation de sol_temp"]

for i in range(8):
    plt.figure()
    plt.plot(p,t[i])
    plt.grid(True)
    plt.title(n[i])
    plt.xlabel("Nombre de coeurs")
    plt.ylabel("Temps d'execution (s)")
    plt.savefig(str(i+1) + ".jpg")
