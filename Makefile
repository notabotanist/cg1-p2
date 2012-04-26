#////////////////////////////////////////////////////////////
#//
#// File:  Makefile
#// Authors:  G. Fotiades, A. Koutmos
#// Contributors:
#// Last modified: 2/2/11
#//
#// Description:  Makefile, targets:
#//               Solaris Build:
#//                 - make
#//                 - make all
#//                 - make tessellation
#//               OS X Build:
#//                 - make mac
#//               Cleanup:
#//                 - make clean
#//                 - make realclean
#//
#////////////////////////////////////////////////////////////

LDLIBS = -lglut -lGLU -lGL -lXext -lX11 -lm
MACLDLIBS = -framework GLUT -framework OpenGL -framework Cocoa

SOURCE_C = tessellation.cpp input.cpp renderings.cpp
SOURCE_H = input.h resources.h vecmath.h
OBJ_FILES = tessellation.o input.o renderings.o
BINARY = tessellation

CC =       gcc
CXX =      g++

COMPILER_FLAGS = -g

all:	tessellation

tessellation: $(SOURCE_H) $(SOURCE_C)
	$(CXX) $(COMPILER_FLAGS) -c $(SOURCE_C)
	$(CXX) $(LDLIBS) -o $(BINARY) $(OBJ_FILES)

mac: $(SOURCE_H) $(SOURCE_C)
	$(CXX) $(COMPILER_FLAGS) -c $(SOURCE_C)
	$(CXX) $(MACLDLIBS) -o $(BINARY) $(OBJ_FILES)

# Dependencies
tessellation.o: input.o
input.o: input.h resources.h

clean:
	rm -rf $(OBJ_FILES) $(BINARY) ptrepository SunWS_cache .sb ii_files core 2> /dev/null

realclean:       clean
