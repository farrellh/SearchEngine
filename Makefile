
CXX = g++
CPPFLAGS = -g -Wall -std=c++11

all: search


search: searchengine.o webpage.o search.cpp
	$(CXX)  $(CPPFLAGS) $^ -o $@

searchengine.o: searchengine.cpp searchengine.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

webpage.o: webpage.cpp webpage.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f search
	rm -f searchengine.o
	rm -f webpage.o