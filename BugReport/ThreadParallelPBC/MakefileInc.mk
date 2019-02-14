PS_PATH = ../../src

USERLIB = -lm -lpthread 
USERINCLUDE =  -I$(PS_PATH) 

CXX=mpicxx
LINK = $(CXX)

# optimized
# CXXFLAGS= -O3 -march=native -qopenmp
# LINKFLAGS= $(CXXFLAGS) -lm -ldl

# debug
CXXFLAGS = -std=c++14 -O3 -qopenmp -std=c++14
LINKFLAGS= $(CXXFLAGS)

# enable openmp and mpi
CXXFLAGS += -DPARTICLE_SIMULATOR_THREAD_PARALLEL
CXXFLAGS += -DPARTICLE_SIMULATOR_MPI_PARALLEL
