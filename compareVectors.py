import sys
import numpy as np

Args = sys.argv
AX = open(Args[1],"r").readlines()
AX = [float(a) for a in AX]
B = open(Args[2],"r").readlines()
B = [float(b) for b in B]

if (len(AX) == len(B)):
    print "Taille de AX = Taille de B"
else:
    print "Difference de tailles"

print "AX = " + str(AX[:4])
print "B = " + str(B[:4])

RMS = np.sqrt(np.sum([(a-b)**2 for a,b in zip(AX,B)]))
print "RMS = " + str(RMS)
