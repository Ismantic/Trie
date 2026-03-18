CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: example double_array_test

example: example.cc trie.cc trie.h
	$(CXX) $(CXXFLAGS) -o $@ example.cc trie.cc

double_array_test: double_array_test.cc double_array.h
	$(CXX) $(CXXFLAGS) -o $@ double_array_test.cc

test: double_array_test
	./double_array_test

clean:
	rm -f example double_array_test *.o

.PHONY: all test clean
