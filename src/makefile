CXX=g++

CXXFLAGS=-I $(IDIR) -std=c++11 -O2 -g

OS := $(shell uname)

ifeq ($(OS),Darwin)
	CXXLIBFLAGS=-dynamiclib -flat_namespace
else
	CXXLIBFLAGS=-fPIC -shared
endif

ODIR=../obj
LDIR=../lib
TDIR=../test
SDIR=.
IDIR=../include

_DEPS=MuskMemoryManager.hpp
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=MuskMemoryManager.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TEST_SOURCES=$(TDIR)/BasicMMOps.cpp
TEXE = $(patsubst %.cpp,%,$(TEST_SOURCES))

_LIB=libMuskMemoryManager.so
LIB = $(patsubst %,$(LDIR)/%,$(_LIB))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

MuskMemoryManager: $(OBJ)
	$(CXX) $(CXXFLAGS) $(CXXLIBFLAGS) -o $(LIB) $(OBJ)

test: $(TEXE)

$(TEXE): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -L $(LDIR) $< -o $@ -lMuskMemoryManager

.PHONY: clean

clean:
	rm $(ODIR)/*.o $(LDIR)/*

testclean:
	rm $(TEXE)
