CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

all: trie-1-test trie-2-test

trie-1-test: trie-1-test.cc trie-1.cc trie-1.h
	$(CXX) $(CXXFLAGS) -o $@ trie-1-test.cc trie-1.cc

trie-2-test: trie-2-test.cc trie-2.h
	$(CXX) $(CXXFLAGS) -o $@ trie-2-test.cc

test: trie-1-test trie-2-test
	./trie-1-test
	./trie-2-test

clean:
	rm -f example double_array_test trie-1-test trie-2-test *.o

.PHONY: all test clean
