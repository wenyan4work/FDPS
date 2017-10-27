
PS_PATH = ../../src

USERLIB = -lm -lpthread 
USERINCLUDE =  -I$(PS_PATH) 

CXX=mpicxx
LINK = $(CXX)

# for MacPorts gcc with clang asm
CXX += -Wa,-q

# optimized
CXXFLAGS= -O3 -fopenmp -march=native
LINKFLAGS= $(CXXFLAGS) -lm -ldl

# debug
#CXXFLAGS = -std=c++11 -O0 -fopenmp -g #-DZDDDEBUG -DIFPACKDEBUG #-DMYDEBUGINFO -DPARTICLE_SIMULATOR_DEBUG_PRINT -DHYRDRODEBUG
#LINKFLAGS= $(CXXFLAGS)

# enable openmp and mpi
CXXFLAGS += -DPARTICLE_SIMULATOR_THREAD_PARALLEL -fopenmp
CXXFLAGS += -DPARTICLE_SIMULATOR_MPI_PARALLEL
