PS_PATH  = -I../../../src

CCC = mpicxx
CFLAGS = -O3 -march=native  #-DMPICH_IGNORE_CXX_SEEK # -ffast-math -funroll-loops -DMPICH_IGNORE_CXX_SEEK #-Wall
CFLAGS += -DPARTICLE_SIMULATOR_THREAD_PARALLEL -fopenmp
CFLAGS += -DPARTICLE_SIMULATOR_MPI_PARALLEL

# for macports gcc with clang asm
# CFLAGS += -Wa,-q

check:
	make -C DomainInfo CCC="$(CCC)" CFLAGS="$(CFLAGS)" PS_PATH="$(PS_PATH)"
	make -C ParticleSystem CCC="$(CCC)" CFLAGS="$(CFLAGS)" PS_PATH="$(PS_PATH)"

clean:
	rm -f *~
	make -C DomainInfo clean
	make -C ParticleSystem clean

distclean:
	rm -f *~
	make -C DomainInfo distclean
	make -C ParticleSystem distclean

allclean:
	rm -f *~
	make -C DomainInfo allclean
	make -C ParticleSystem allclean

# information for MPICH_IGNORE_CXX_SEEK
# https://software.intel.com/en-us/articles/intel-mpi-library-for-linux-running-list-of-known-issues#A3
