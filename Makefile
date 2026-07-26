CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Isrc

all: trie-1-test trie-2-test

trie-1-test: src/trie-1-test.cc src/trie-1.cc src/trie-1.h
	$(CXX) $(CXXFLAGS) -o $@ src/trie-1-test.cc src/trie-1.cc

trie-2-test: src/trie-2-test.cc src/trie-2.h
	$(CXX) $(CXXFLAGS) -o $@ src/trie-2-test.cc

test: trie-1-test trie-2-test
	./trie-1-test
	./trie-2-test

clean:
	rm -f example double_array_test trie-1-test trie-2-test *.o

.PHONY: all test clean
