import csr
import sys
nnz = int(sys.argv[1])
size = 2e7
density = nnz/(size**2)
string = '%.0e' % nnz
csr.createDataFiles(size,density, matrixFile = "/work/attouche/matrix_" + string, vectorFile = "/work/attouche/vector_" + string, method = "binary")
