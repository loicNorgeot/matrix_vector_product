# -*- coding: utf8 -*-

import numpy as np
import scipy as sc
from scipy import sparse as sp
import array

dataFolder = "/work/attouche/"

"""""""""""""""""""""
       MATRICES
"""""""""""""""""""""

def getCSRMatrixFromOldFormat(fileName):
    #Ouverture du fichier
    with open(fileName) as f:
        L = f.readlines()
        print "Success opening the file"

    #Interprétation et suppression des premières lignes
    l0 = L[0].split(" ")
    nRows = int(l0[0])
    nCols = int(l0[1])
    nnz = int(l0[2])
    print nRows, nCols, nnz
    L = L[3:]

    #Enregistrement des indices colonne dans A des premiers non nuls de chaque ligne
    IA = np.concatenate([[int(l) for l in L[i].split(" ")] for i in range(nRows/10)])
    L = L[nRows/10:]
    print "Après enregistrement de IA:\n", L[0], "\n"
    
    #Enregistrement des indices colonne de chaque non nuls
    JA = np.concatenate([[int(l) for l in L[i].split(" ")] for i in range(nnz/10)])
    L = L[nnz/10:]
    print "Après enregistrement de JA:\n", L[0], "\n"

    #Enregistrement des non nuls sous la forme de float
    A = np.concatenate([[float(l) for l in L[i].split(" ")] for i in range(nnz/10)])
    
    print len(A), len(IA), len(IA)
    return A, IA, JA

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
    #Retour de A, IA, JA
    return A, IA, JA, nRows, nnz


def writeCSRMatrixInOldFormat(A,IA,JA,fileName):
    #Ouverture du fichier
    newFile = open(fileName,"w")
    
    #Ecriture du header sur les premières lignes
    #La syntaxe exacte est sur la ligne qui suit:
    #nRows nCols nnz \n
    #\n
    #\n
    newFile.write(str(len(IA)-1) + " " + str(len(IA)-1) + " " + str(len(JA)) + " \n")
    newFile.write("\n")
    newFile.write("\n")
    
    #Ecriture 10 par 10 de IA, JA et A
    for i,x in enumerate(IA):
        if((i%10!=0) & (i!=len(IA)-1)):
            newFile.write(str(x) + " ")
        if((i%10!=0) & (i==len(IA)-1)):
            newFile.write(str(x) + "\n")
        if((i%10==0) & (i!=0)):
            newFile.write(str(x)+"\n")
        if((i%10==0) & (i==0)):
            newFile.write(str(x)+" ")

    for i,x in enumerate(JA):
        if((i%10!=0) & (i!=len(JA)-1)):
            newFile.write(str(x) + " ")
        if((i%10!=0) & (i==len(JA)-1)):
            newFile.write(str(x) + "\n")
        if((i%10==0) & (i!=0)):
            newFile.write(str(x)+"\n")
        if((i%10==0) & (i==0)):
            newFile.write(str(x)+" ")

    for i,x in enumerate(A):
        if((i%10!=0) & (i!=len(A)-1)):
            newFile.write(str(x) + " ")
        if((i%10!=0) & (i==len(A)-1)):
            newFile.write(str(x) + "\n")
        if((i%10==0) & (i!=0)):
            newFile.write(str(x)+"\n")
        if((i%10==0) & (i==0)):
            newFile.write(str(x)+" ")
    
    #Fermeture du fichier
    newFile.close()

def writeCSRMatrixInNewFormat(A,IA,JA,fileName):
    #Ouverture du fichier
    newFile = open(fileName,"w")
    
    #Ecriture du header sur la première ligne
    #La syntaxe exacte est sur la ligne qui suit:
    #nombreDeLignes nombreDElementsNonZeros \n
    newFile.write(str(len(IA)-1) + " " + str(len(JA)) + " \n")
    
    #Ecriture sur trois lignes de A, IA et JA
    #Chaque elt est séparé du suivant par un espace
    #La séparation entre les trois tableaux est 
    #représentée par un saut de ligne
    for i in A:
        newFile.write(str(i) + " ")
    newFile.write("\n")
    for i in IA:
        newFile.write(str(i) + " ")
    newFile.write("\n")
    for i in JA:
        newFile.write(str(i) + " ")
    newFile.write("\n")
    
    #Fermeture du fichier
    newFile.close()
    



"""""""""""""""""""""
       VECTEURS
"""""""""""""""""""""

def getCSRVectorFromOldFormat(fileName):
    with open(fileName) as f:
            L = f.readlines()
            print "Success opening the file"
    nRowsVec = int(L[0][:-1])
    Vecteur = [float(l[:-1]) for l in L[1:]]
    print len(Vecteur), nRowsVec
    return Vecteur, nRowsVec

def getCSRVectorFromNewFormat(fileName):
    #Ouverture du fichier
    with open(fileName) as f:
        L = f.readlines()
        print "Success opening the file"
        
    nRows = int(L[0].split(" ")[0])

    X = [float(l) for l in L[1].split(" ")[:-1]]
    return X,nRows

def writeCSRVectorInOldFormat(Vec,fileName):
    #Ouverture du fichier
    newFile = open(fileName,"w")
    #Ecriture du nombre d'éléments sur la première ligne
    newFile.write(str(len(Vec)) + " \n")
    for i in Vec:
        newFile.write(str(i) + "\n")
    newFile.close()

def writeCSRVectorInNewFormat(Vec,fileName):
    #Ouverture du fichier
    newFile = open(fileName,"w")
    #Ecriture du nombre d'éléments sur la première ligne
    newFile.write(str(len(Vec)) + " \n")
    for i in Vec:
        newFile.write(str(i) + " ")
    newFile.close()

def writeCSRVectorInBinaryFormat(Vec, fileName):
    file = open(fileName + "_V.bin","wb")
    V = array.array("d")
    V.fromlist(Vec.tolist())
    V.tofile(file)
    fileHeader = open(fileName + "_H.data","w")
    fileHeader.write(str(len(Vec)))

def writeCSRMatrixInBinaryFormat(A,IA,JA,fileName):
    #Ouverture du fichier
    headerFile = open(fileName + "_H.data","w")
    headerFile.write(str(len(IA)-1) + " " + str(len(JA)))
    headerFile.close()
    
    #Ecriture de A en binaire
    aFile = open(fileName + "_A.bin", "wb")
    a = array.array('d')
    a.fromlist(A.tolist())
    a.tofile(aFile)
    aFile.close()
    
    #Ecriture de IA en binaire
    iaFile = open(fileName + "_IA.bin", "wb")
    ia = array.array('i')
    ia.fromlist(IA.tolist())
    ia.tofile(iaFile)
    iaFile.close()
    
    #Ecriture de JA en binaire
    jaFile = open(fileName + "_JA.bin", "wb")
    ja = array.array('i')
    ja.fromlist(JA.tolist())
    ja.tofile(jaFile)
    jaFile.close()
    


"""""""""""""""""""""
       CLASSES
"""""""""""""""""""""
def _gen_unique_rand(rng, _gk):
    ind = rng.rand(int(_gk))
    u = np.unique(np.floor(ind * mn))
    rng.shuffle(u)
    return u[:k]

class CSR:
    def __init__(self, s,d):
        X = sp.rand(s,s,d,format='csr')
        self.nnz = X.nnz
        self.nRows = X.shape[0]
        self.A = X.data
        self.IA = X.indptr
        self.JA = X.indices
        
    def display(self):
        print self.nnz, self.nRows
        print self.A
        print self.IA
        print self.JA
        
    def write(self, fileName, method):
        if(method == "new"):
            writeCSRMatrixInNewFormat(self.A, self.IA, self.JA, fileName)
        if (method == "old"):
            writeCSRMatrixInOldFormat(self.A, self.IA, self.JA, fileName)
        if (method == "binary"):
            writeCSRMatrixInBinaryFormat(self.A, self.IA, self.JA, fileName)
        
class VEC:
    def __init__(self, size):
        X = sc.rand(size)
        self.vector = X
    
    def write(self, fileName, method):
        if(method == "new"):
            writeCSRVectorInNewFormat(self.vector,fileName)
        if(method == "old"):
            writeCSRVectorInOldFormat(self.vector,fileName)
        if(method == "binary"):
            writeCSRVectorInBinaryFormat(self.vector,fileName)



"""""""""""""""""""""
      FONCTIONS
"""""""""""""""""""""
        
def createDataFiles(size, density, matrixFile = dataFolder + "matrix", vectorFile = dataFolder + "vector", method="new"):
    if(density*size**2 < 2e9 + 1):
        print "Taille acceptée"
        M = CSR(size,density)
        M.write(matrixFile,method)
        V = VEC(size)
        V.write(vectorFile,method)
    else:
        print "Taille trop importante"

def newToOld(matrixFile, vectorFile, destMatrix = "/work/attouche/convertedMatrix.data", destVector = "/work/attouche/convertedVector.data"):
    X, nR = getCSRVectorFromNewFormat(vectorFile)
    writeCSRVectorInOldFormat(X,destVector)
    del X
    print "Conversion du vecteur"

    A,IA,JA,a,b = getCSRMatrixFromNewFormat(matrixFile)
    writeCSRMatrixInOldFormat(A,IA,JA,destMatrix)
    print "Conversion de la matrice"
