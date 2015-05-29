#!/usr/local/bin/python
# -*- coding: utf-8 -*-
import numpy as np
import os
import array

def readCSRMatrixFromOldFormat(fileName):
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
            print i, k, JA[k]
            print X[JA[k]]
            break
    return V


#############
# EXECUTION #
#############

def HarwellToBinary(inFile,
                    inPath=os.getcwd(),
                    outPath=os.getcwd()):
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
    #Ecriture
    X = np.ones(len(IA)-1)
    V = multiply(IA, JA, A, X)
    writeBinaryVector(V, name, outPath)
    writeBinaryCSRMatrix(A, IA, JA, name, outPath)

def PascalToBinary(inFileMatrix,
                   inFileVector,
                   inPath=os.getcwd(),
                   outPath=os.getcwd()):
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
    #Ecriture des tableaux en binaire
    name = "pascal"
    writeBinaryCSRMatrix(A, IA, JA, name, outPath)
    #Lecture du vecteur
    vectorFile = inPath + "/" + inFileVector
    V = open(vectorFile, 'r').readlines()
    V = np.array([float(v) for v in V[1:]])
    print len(V)
    #Ecriture du vecteur en binaire
    writeBinaryVector(V, name, outPath)

PascalToBinary("matrix.data","vector.data", inPath="/home/loic/Documents/Lamia")
    

#HarwellToBinary("s3dkq4m2.dat")
#HarwellToBinary("s1rmq4m1.dat")
#HarwellToBinary("36.dat")
#HarwellToBinary("bcsstk09.dat")
#HarwellToBinary("bcsstk04.dat")
#HarwellToBinary("bcsstk01.dat")
#HarwellToBinary("1138.dat")
#HarwellToBinary("bcsstm23.dat")
#HarwellToBinary("bcsstm01.dat")
