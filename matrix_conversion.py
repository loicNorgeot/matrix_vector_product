#!/usr/local/bin/python
# -*- coding: utf-8 -*-
import numpy as np
import os
import array
import matplotlib.pyplot as plt


def getFiles():
    bannedFiles=["nos3.dat"]
    F = os.listdir(os.getcwd())
    F = [f for f in F if ".dat" in f and '.data' not in f and f not in bannedFiles]
    return F

def unique_rows(a):
    a = np.ascontiguousarray(a)
    unique_a = np.unique(a.view([('', a.dtype)]*a.shape[1]))
    return unique_a.view(a.dtype).reshape((unique_a.shape[0], a.shape[1]))

######################################
# LECTURE A PARTIR DU FORMAT HARWELL #
######################################

def readHeader(lines):
    H = [lines[i].split() for i in range(4)]
    #Première ligne
    title = lines[0][:72]
    key = lines[0][72:]
    #Deuxième ligne
    nLTot = int(H[1][0])
    nLIA = int(H[1][1])
    nLJA = int(H[1][2])
    nLA = int(H[1][3])
    nLVector = int(H[1][4])
    #Troisième ligne
    matrixType = H[2][0]
    nRows = int(H[2][1])
    nCols = int(H[2][2])
    nnz = int(H[2][3])
    return nLIA, nLJA, nLA

def linesToInt(lines, 
               numberOfLines):
    return np.concatenate([[int(l) for l in lines[i].split()] for i in range(numberOfLines)])

def linesToFloat(lines, 
                 numberOfLines):
    return np.concatenate([[float(l) for l in lines[i].split()] for i in range(numberOfLines)])


#######################
# ECRITURE EN BINAIRE #
#######################

def writeBinaryArray(X, 
                     path, 
                     dataType='i'):
    File = open(path, "wb")
    x = array.array(dataType)
    x.fromlist(X.tolist())
    x.tofile(File)
    File.close()

def writeBinaryCSRMatrix(A,
                         IA,
                         JA,
                         name,
                         path):
    #Ecriture du header
    headerFile = open(path + "/matrix_" + name + "_H.data","w")
    headerFile.write(str(len(IA)-1) + " " + str(len(JA)))
    headerFile.close()
    #Ecriture des tableaux
    writeBinaryArray(A, path + "/matrix_" + name + "_A.bin", dataType='d')
    writeBinaryArray(IA, path + "/matrix_" + name + "_IA.bin", dataType='i')
    writeBinaryArray(JA, path + "/matrix_" + name + "_JA.bin", dataType='I')

def writeBinaryVector(Vec,
                      name, 
                      path):
    fileHeader = open(path + "/vector_" + name + "_H.data","w")
    fileHeader.write(str(len(Vec)))
    fileHeader.close()
    writeBinaryArray(Vec, path + "/vector_" + name + "_V.bin", dataType='d')


###################################
# CONVERSION EN DENSE ET SYMETRIE #
###################################

def symetrize(IA,
              JA,
              A,
              sym = True,
              plot = True):
    nR = len(IA) - 1

    #Création d'une matrice dense
    MAT = []
    for i in range(nR):
        for j in range(IA[i], IA[i+1]):
            k = JA[j]
            MAT.append((i, k, A[j]))
            if(sym):
                MAT.append((k,i,A[j]))
    MAT.sort()
    MAT = unique_rows(MAT)

    #Passage en CSR
    ia = 0
    IA = []
    JA = []
    A = []
    ia_prev = -1
    for i,m in enumerate(MAT):
        ia = m[0]
        if ia!=ia_prev:
            IA.append(i)
            ia_prev = ia
        A.append(m[2])
        JA.append(int(m[1]))
    IA.append(len(JA))
    IA = np.array(IA)
    JA = np.array(JA)
    A = np.array(A)

    if(plot):
        plt.figure(figsize=(12,12))
        X = MAT[:,0]
        Y = MAT[:,1]
        Z = MAT[:,2]
        if(len(X)!=len(Y)):
            print len(X), len(Y)
            print "ERREUR FATALE!!"
        plt.scatter(X,
                    -Y + np.max(Y),
                    60,
                    c=np.log10(Z),
                    vmin = -0.5,
                    vmax = 2.5,
                    cmap='jet',
                    alpha=0.3,
                    lw=0)
        plt.colorbar()
        plt.xlim(0,len(IA))
        plt.ylim(0,len(IA))
        #plt.savefig(str(len(JA)) + '.jpg')
        plt.savefig('toto.jpg')

    return IA, JA, A


#################
# CREATION DE B #
#################

def multiply(IA,
             JA,
             A,
             X):
    nR = len(IA)-1
    V = np.zeros(nR)
    for i in range(nR):
        k=0
        try:
            tmp = 0
            for j in range(IA[i],IA[i+1]):
                k=j
                tmp += A[j] * X[JA[j]]
            V[i] = tmp
        except:
            print i, k, IA[i], IA[i+1], JA[k]
            print X[JA[k]]
            break
    return V


#############
# EXECUTION #
#############

def HarwellToBinary(inFile,
                    inPath=os.getcwd(),
                    outPath=os.getcwd(),
                    sym=False):
    #Lecture
    file = inPath + "/" + inFile
    L = open(file,"r").readlines()
    name = inFile.split(".")[0]

    #Header
    nLIA, nLJA, nLA = readHeader(L)

    #Tableaux
    IA = linesToInt(L[4:], nLIA)
    JA = linesToInt(L[4+nLIA:], nLJA)
    A = linesToFloat(L[4+nLIA+nLJA:], nLA)
    JA-=1
    IA-=1

    print "Taille de JA (=nnz) = " + str(len(JA))
    print "Dernier IA (=nnz en theorie) = " + str(IA[-1])

    if(sym):
        IA, JA ,A = symetrize(IA,JA,A)
    
    #Ecriture
    X = np.ones(len(IA)-1)
    V = multiply(IA, JA, A, X)
    writeBinaryVector(V, name, outPath)
    writeBinaryCSRMatrix(A, IA, JA, name, outPath)


def PascalToBinary(inFileMatrix,
                   inFileVector,
                   inPath=os.getcwd(),
                   outPath=os.getcwd(),
                   sym=False):
    #Ouverture
    matrixFile = inPath + "/" + inFileMatrix
    L = open(matrixFile,'r').readlines()

    #Calcul du nombre de lignes pour chaque tableau
    nRows = int(L[0].split(" ")[0])
    nnz = int(L[0].split(" ")[2])
    nLIA = (nRows + 1)/10 + 1#Attention!!!! Pas correct pour les chiffres ronds!!!!!
    nLJA = nnz/10 + 1
    nLA = nnz/10 + 1

    #Lecture des tableaux
    IA = linesToInt(L[3:], nLIA)
    JA = linesToInt(L[3+nLIA:], nLJA)
    A = linesToFloat(L[3+nLIA+nLJA:], nLA)
    print len(IA), len(JA), len(A)
    print nRows, nnz

    if sym:
        IA, JA ,A = symetrize(IA,JA,A)

    #Ecriture des tableaux en binaire
    name = "pascal"
    writeBinaryCSRMatrix(A, IA, JA, name, outPath)

    #Lecture du vecteur
    vectorFile = inPath + "/" + inFileVector
    V = open(vectorFile, 'r').readlines()
    V = np.array([float(v) for v in V[1:]])
    #Ecriture du vecteur en binaire
    writeBinaryVector(V, name, outPath)
    

#HarwellToBinary("s3dkq4m2.dat", sym = True)
PascalToBinary("matrix.data","vector.data", inPath="/home/loic/Documents/Lamia", sym=True)
