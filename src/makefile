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

MM_SOURCES=$(wildcard $(SDIR)/*.cpp)
MM_LIBS=$(patsubst $(SDIR)/%.cpp,$(LDIR)/lib%.so,$(MM_SOURCES))

TEST_SOURCES=$(wildcard $(TDIR)/*.cpp)
TEXE = $(patsubst %.cpp,%,$(TEST_SOURCES))

_LIB=libMuskMemoryManager.so
LIB = $(patsubst %,$(LDIR)/%,$(_LIB))

all: $(MM_LIBS) test

$(LDIR)/lib%.so: $(ODIR)/%.o
	$(CXX) $(CXXFLAGS) $(CXXLIBFLAGS) -o $@ $<

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test: $(TEXE)

$(TDIR)/%: $(TDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -L $(LDIR) $< -o $@ -lMuskMemoryManager
	$(CXX) $(CXXFLAGS) -L $(LDIR) $< -o $@EX -lMuskMemoryManagerEX

.PHONY: clean

veryclean:
	rm -r $(ODIR)/*.o $(LDIR)/*
	rm $(TEXE)

clean:
	rm -r $(ODIR)/*.o $(LDIR)/*

testclean:
	rm $(TEXE)
