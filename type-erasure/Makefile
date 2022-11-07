# Makefile for type-erasure.

CXX = g++
CXXFLASGS += -std=c++20 -I..

all: main

clean:
	$(RM) *.o main type-erasure

run: main
	./main

run-type-erasure: type-erasure
	./type-erasure

circle.o: circle.cc circle.h
	$(CXX) $(CXXFLASGS) -c $<

square.o: square.cc square.h
	$(CXX) $(CXXFLASGS) -c $<

main.o: main.cc shape.h square.h circle.h
	$(CXX) $(CXXFLASGS) -c $<

main: main.o circle.o square.o
	$(CXX) $(CXXFLASGS) $^ -o $@

type-erasure.o: type-erasure.cc
	$(CXX) $(CXXFLASGS) -c $<

type-erasure: type-erasure.o
	$(CXX) $(CXXFLASGS) $^ -o $@
