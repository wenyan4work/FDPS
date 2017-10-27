
PS_PATH = ../../src

USERLIB = -lm -lpthread 
USERINCLUDE =  -I$(PS_PATH) 

CXX=g++-7
LINK = $(CXX)

# optimized
#CXXFLAGS= -O3 -fopenmp -march=native
#LINKFLAGS= -O3 -fopenmp -march=native -lm -ldl

# debug
CXXFLAGS = -std=c++11 -O0 -fopenmp -g #-DZDDDEBUG -DIFPACKDEBUG #-DMYDEBUGINFO -DPARTICLE_SIMULATOR_DEBUG_PRINT -DHYRDRODEBUG
LINKFLAGS= -fopenmp -g 
