# -*- coding: utf8 -*-
import numpy as np
import matplotlib.pyplot as plt

def getCSRVectorFromOldFormat(fileName):
    with open(fileName) as f:
            L = f.readlines()
            print "Success opening the file"
    nRowsVec = int(L[0][:-1])
    Vecteur = [float(l[:-1]) for l in L[1:]]
    print len(Vecteur), nRowsVec
    return Vecteur, nRowsVec

def getCSRMatrixFromOldFormat(fileName):
    #Ouverture du fichier
    with open(fileName) as f:
        L = f.readlines()
        print "Success opening the file"
    
    #Interprétation et suppression des premières lignes
    l0 = L[0].split(" ")
    nRows = int(l0[0])
    nCols = int(l0[1])
    nnz = int(l0[2][:-1])
    nRows, nCols, nnz
    L = L[3:]
    
    #Enregistrement des indices colonne dans A des premiers non nuls de chaque ligne
    IA = np.concatenate([[int(l) for l in L[i].split(" ")[:-1]] for i in range(nRows/10+1)])
    L = L[nRows/10 + 1 :]
    
    #Enregistrement des indices colonne de chaque non nuls
    JA = np.concatenate([[int(l) for l in L[i].split(" ")[:-1]] for i in range(nnz/10+1)])
    L = L[nnz/10+1:]
    
    #Enregistrement des non nuls sous la forme de float
    A = np.concatenate([[float(l) for l in L[i].split(" ")[:-1]] for i in range(nnz/10+1)])
    
    #Retour de A, IA, JA
    return A, IA, JA

def getCSRVectorFromNewFormat(fileName):
    #Ouverture du fichier
    with open(fileName) as f:
        L = f.readlines()
        print "Success opening the file"
    nRows = int(L[0].split(" ")[0])
    X = [float(l) for l in L[1].split(" ")[:-1]]
    #Retour de A, IA, JA
    return X,nRows

def getCSRMatrixFromNewFormat(fileName):
    #Ouverture du fichier
    with open(fileName) as f:
        L = f.readlines()
        print "Success opening the file"
    nRows = int(L[0].split(" ")[0])
    nnz = int(L[0].split(" ")[1])
    A = [float(l) for l in L[1].split(" ")[:-1]]
    IA = [int(l) for l in L[2].split(" ")[:-1]]
    JA = [int(l) for l in L[3].split(" ")[:-1]]
    return A, IA, JA, nRows, nnz

def checkProduct(matrixFile="matrix.data", vectorFile="vector.data",resultFile="res.data",method="new"):
    if(method=="new"):
        #Import de la matrice et du vecteur
        vec = getCSRVectorFromNewFormat(vectorFile)[0]
        A, IA, JA,x,y = getCSRMatrixFromNewFormat(matrixFile)
    else:
        vec = getCSRVectorFromOldFormat(vectorFile)[0]
        A, IA, JA = getCSRMatrixFromOldFormat(matrixFile)

    #Import du vecteur resultat
    res = open(resultFile,"r").readlines()
    res = [float(x[:-1]) for x in res]

    print len(res), len(vec), len(A), len(IA), len(JA)

    
    #calcul du produit CSR
    sol = np.zeros(len(IA)-1)
    for i in range(len(IA)-1):
        sol[i] = sum([A[j]*vec[JA[j]] for j in range(IA[i], IA[i+1])])
    print len(sol)

    #retour de la comparaison
    return sol, res


sol, res = checkProduct(matrixFile='newMatrix8.data', vectorFile = 'newVector8.data',resultFile="res.data")
diff = [s-r for s,r in zip(sol,res)]
plt.figure()
plt.hist(diff,100,label="dev = " + str(np.std(diff)))
plt.legend()
plt.savefig('comp.jpg')
