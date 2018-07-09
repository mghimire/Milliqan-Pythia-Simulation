SHELL=/usr/bin/env bash

# PREFIX_LIB=/cvmfs/sft.cern.ch/lcg/external/MCGenerators_lcgcmt67c/pythia8/201/x86_64-slc6-gcc48-opt/lib
# PREFIX_INCLUDE=/cvmfs/sft.cern.ch/lcg/external/MCGenerators_lcgcmt67c/pythia8/201/x86_64-slc6-gcc48-opt/include
PREFIX_LIB=/Users/gregory/Documents/classes/nyu/2018_summer/pythia/pythia8235/lib
PREFIX_INCLUDE=/Users/gregory/Documents/classes/nyu/2018_summer/pythia/pythia8235/include

ROOT_INCLUDE=$(shell root-config --incdir)

CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON)
CXX_COMMON+= -L$(PREFIX_LIB) -Wl,-rpath,$(PREFIX_LIB) -lpythia8 -ldl
CXX_COMMON+= -std=c++11 -O2 -march=native

ROOTBIN=/usr/bin/

.SECONDEXPANSION:
.PHONY: all clean

all: py_sim

py_sim: $$@.cc $(PREFIX_LIB)/libpythia8.a
	$(CXX) $< -o $@ -w -I$(ROOT_INCLUDE) $(CXX_COMMON)\
	 $(shell root-config --cflags)\
	 -Wl,-rpath,$(ROOT_LIB) $(shell root-config --glibs)

clean:
	@rm -f py_sim;\
	rm -f *~; rm -f *.so; rm -f *.pcm; rm -f *.d
