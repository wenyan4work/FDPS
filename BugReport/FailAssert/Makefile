
include MakefileInc.mk

# 
INCLUDE_DIRS =  $(USERINCLUDE)
LIBRARY_DIRS = 
LIBRARIES = $(USERLIB)

# System-specific settings
SHELL = /bin/sh
SYSLIB =	
SIZE =	size


# Files

SRC =   main.cpp 
INC =   
         
# Definitions

EXE =   SphereTest.X
OBJ =   $(SRC:.cpp=.o)

# Link rule
$(EXE):	$(OBJ)
	$(LINK) $(OBJ)  -o $(EXE) $(LINKFLAGS) $(SYSLIB) $(LIBRARY_DIRS) $(LIBRARIES)
	$(SIZE) $(EXE)

# Compilation rules
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $*.cpp -o $*.o

# Individual dependencies
$(OBJ): $(INC)

all: $(EXE)

clean: 
	rm ./$(OBJ)
	rm ./$(EXE)
