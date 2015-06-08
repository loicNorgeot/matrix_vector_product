#Conjugate Gradient
ifeq (${ARG},conjugate)
  SRC= vector.cpp csrmatrix.cpp matrix.cpp gc.cpp binaryIO.cpp chrono.cpp
  MAINFILE=mainGC

#Simple product
else ifeq (${ARG},product)
  SRC= binaryIO.cpp chrono.cpp
  MAINFILE=mainProduct

#OMP
else ifeq (${ARG},openmp)
  SRC= vector.cpp csrmatrix.cpp binaryIO.cpp
  MAINFILE=mainOpenMP

#GMRES
else ifeq (${ARG},gmres)
  SRC = vector.cpp csrmatrix.cpp matrix.cpp chrono.cpp binaryIO.cpp gmres.cpp
  MAINFILE=mainGMRES

else
  echo "PROBLEME!!!" 

endif

CXX=icpc
CXXFLAGS=-fopenmp -O3 -lm -mkl
EXEC=main
OBJ= $(SRC:.cpp=.o)
HEA= $(SRC:.cpp=.h)

all: $(EXEC)

main : $(OBJ) $(MAINFILE).o
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp %.h
	$(CXX) -c $< $(CXXFLAGS)

$(MAINFILE).o : $(MAINFILE).cpp $(HEA)
	$(CXX) -c $< $(CXXFLAGS)


.PHONY: clean mrproper
clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)	  
