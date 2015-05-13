import csr
import sys
nnz = int(sys.argv[1])
size = 1e6
density = nnz/(size**2)
string = '%.0e' % nnz
csr.createDataFiles(size,density, matrixFile = "/work/norgeot/matrix_" + string, vectorFile = "/work/norgeot/vector_" + string, method = "binary")
