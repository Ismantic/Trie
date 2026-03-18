#include <iostream>
#include <bitset>
#include "trie.h"

void PrintBinary(char c) {
  std::bitset<8> bits(c);
  std::cout << bits << std::endl;
}

void test_critbit_trie() {

  trie::CritbitTrie ct;

  ct.Insert("b");
  ct.Insert("c");
  ct.Insert("d");
  ct.Insert("a");

  //PrintBinary('a');
  //PrintBinary('b');
  //PrintBinary('c');
  //PrintBinary('d');

  ct.Print();

  ct.Insert("ba");
  //ct.Insert("bb");
  //ct.Insert("cc");
  //ct.Insert("dd");

  ct.Insert("baa");
  //ct.Insert("ab");
  //ct.Insert("ac");

  std::cout << ct.Count() << std::endl;

  //ct.PrintCritbit("x");
  //std::cout << std::endl;

  std::vector<std::string> rs = ct.GetValues("c");
  std::cout << "GetValues" << std::endl;
  for (auto s : rs) {
    std::cout << s << " ";
  }
  std::cout << std::endl;

  //ct.PrintCritbit("aa");
  //std::cout << std::endl;
  //ct.PrintCritbit("cc");
  //std::cout << std::endl;


  ct.Print();

  ct.PrintValues();

  std::cout << "Critbit" << std::endl;
  ct.PrintCritbit("baabcd");

  std::vector<std::string> vs = ct.GetCommonValues("baabd");
  for (auto s : vs) {
    std::cout << s << " ";
  }
  std::cout << std::endl;

}

void test_double_array_trie() {
    trie::DoubleArrayTrie trie;
    std::vector<std::string> strs = {"a", "ab", "abc", "b", "c", "d"};

    trie.Build(strs);

    auto rs = trie.GetValues("a");
    for (const auto& s : rs) {
      std::cout << s << " ";
    }
    std::cout << std::endl;

    auto vs = trie.GetCommonValues("abcd");
    for (const auto& s : vs) {
      std::cout << s << " ";
    }
    std::cout << std::endl;
}

int main() {

  test_critbit_trie();
  test_double_array_trie();
  return 0;

}
