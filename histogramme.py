# -*- coding: utf8 -*-
import matplotlib.pyplot as plt
import numpy as np
import os

def getFileList(directory):
    fileList = os.listdir(directory)
    files = [f for f in fileList if(("computation_times" in f) and (".txt" in f))]
    files = [f for f in sorted(files)]
    return files

def loadFile(fileName):
    with open(fileName) as f:
        X = f.readlines()
    X = np.array(X)
    for i,x in enumerate(X):
        X[i] = x[:-2]
    X = X[1:]
    X = [float(x) for x in X]
    return X

def createArrays(fileList):
    X = [loadFile(f) for f in fileList]
    L = np.array(X)
    T = [f[22:25] + " thread" for f in fileList]
    m = max([np.max(x) for x in L])
    return L, T, m

def hist(L, T, m, step = 1):
    nbMax = len(L)
    plt.figure()
    for (i,l),t in zip(enumerate(L),T):
        for k in range(10):
            if (2**k == i+1):
                plt.hist(l, 100, label=t, range = [0,m], alpha = 0.5)
                plt.plot((np.mean(l),np.mean(l)),(0,10000),"--",lw=2,color="black")
    plt.xlabel("Temps de calcul (s)")
    plt.ylabel("Nombre d'occurences")
    plt.grid(True)
    plt.title(u"Histogramme des temps d'éxécution en fonction du nombre de coeurs")
    plt.grid(True)
    plt.legend()
    plt.ylim(0,0.8*len(L[0]))
    plt.savefig('plot_time_histogram.jpg')

def timePlot(L,m):
    plt.figure()
    numProcs = [i+1 for i,l in enumerate(L)]
    meanTimes = [np.mean(l) for l in L]
    #meanTimes = [np.min(l) for l in L]
    plt.plot(numProcs, meanTimes,'*-',color='red')
    plt.xlabel("Nombre de coeurs")
    plt.ylabel(u"Temps moyen d'éxécution (s)")
    plt.grid(True)
    plt.title(u"Temps d'éxécution en fonction du nombre de coeurs")
    plt.grid(True)
    plt.ylim(0,m)
    plt.savefig('plot_execution_time.jpg')

def speedupPlot(L):
    plt.figure()
    oneCoreTime = np.mean(L[0])
    numProcs = [i+1 for i,l in enumerate(L)]
    speedup = [oneCoreTime/np.mean(l) for l in L]
    #speedup = [oneCoreTime/np.min(l) for l in L]
    plt.plot(numProcs, speedup,'*-',color='red',label='real speedup')
    plt.plot((0,len(L)),(0,len(L)),'--',label="theoretical speedup")
    plt.xlabel("Nombre de coeurs")
    plt.ylabel(u"Valeur du speedup")
    plt.grid(True)
    plt.title(u"Speed-up en fonction du nombre de coeurs")
    plt.grid(True)
    plt.legend(loc=2)
    plt.savefig('plot_speedup.jpg')

fileList = getFileList(os.getcwd())
L, T, max = createArrays(fileList)
hist(L,T,max,1)
timePlot(L,max)
speedupPlot(L)






