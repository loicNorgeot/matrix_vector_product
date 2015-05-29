# -*- coding: utf8 -*-

import numpy as np
import scipy as sc
from scipy import sparse as sp
import array

dataFolder = "/work/norgeot/"

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
    ia = array.array('I')
    ia.fromlist(IA.tolist())
    ia.tofile(iaFile)
    iaFile.close()
    
    #Ecriture de JA en binaire
    jaFile = open(fileName + "_JA.bin", "wb")
    ja = array.array('I')
    ja.fromlist(JA.tolist())
    ja.tofile(jaFile)
    jaFile.close()

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
        
    def write(self, fileName):
        writeCSRMatrixInBinaryFormat(self.A, self.IA, self.JA, fileName)
        
class VEC:
    def __init__(self, size):
        X = sc.rand(size)
        self.vector = X
    
    def write(self, fileName):
        writeCSRVectorInBinaryFormat(self.vector,fileName)

def createDataFiles(size, density, matrixFile = dataFolder + "matrix", vectorFile = dataFolder + "vector"):
    if(density*size**2 < 4e9 + 1):
        print "Taille acceptée"
        M = CSR(size,density)
        M.write(matrixFile)
        V = VEC(size)
        V.write(vectorFile)
    else:
        print "Taille trop importante"
