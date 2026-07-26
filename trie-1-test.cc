#include "trie-1.h"

#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& name) {
    if (condition) {
        ++passed;
    } else {
        ++failed;
        std::cout << "FAIL: " << name << '\n';
    }
}

void test_insert_and_lookup() {
    trie::CritbitTrie trie;

    check(trie.Count() == 0, "new trie is empty");
    check(trie.Insert("app") == 0, "insert app");
    check(trie.Insert("apple") == 1, "insert apple");
    check(trie.Insert("application") == 2, "insert application");
    check(trie.Insert("app") == 0, "duplicate returns existing index");
    check(trie.Count() == 3, "duplicate does not change count");
    check(trie.GetValue(1) == "apple", "lookup by index");

    check(trie.GetValues("app") ==
              std::vector<std::string>({"app", "apple", "application"}),
          "find words with prefix");
    check(trie.GetValues("missing").empty(), "missing prefix");
    check(trie.GetValues("").size() == 3, "empty prefix returns all words");
}

void test_common_values() {
    trie::CritbitTrie trie;
    trie.Insert("a");
    trie.Insert("app");
    trie.Insert("apple");

    check(trie.GetCommonValues("applejack") ==
              std::vector<std::string>({"apple", "app", "a"}),
          "find stored prefixes");
    check(trie.GetCommonValues("banana").empty(), "no common values");
}

void test_lock() {
    trie::CritbitTrie trie;
    trie.Insert("existing");

    check(!trie.Lock(true), "lock returns previous state");
    check(trie.IsLock(), "trie is locked");
    check(trie.Insert("new") == static_cast<uint64_t>(-1),
          "locked trie rejects new value");
    check(trie.Insert("existing") == 0, "locked trie finds existing value");
    check(trie.Count() == 1, "locked insertion does not change count");
}

void test_save_load_and_move() {
    trie::CritbitTrie source;
    source.Insert("");
    source.Insert("source");

    std::stringstream buffer;
    source.Save(buffer);

    trie::CritbitTrie loaded;
    loaded.Load(buffer);
    check(loaded.Count() == 2, "load restores count");
    check(loaded.GetValue(0).empty(), "load restores empty string");
    check(loaded.GetValue(1) == "source", "load restores value");

    trie::CritbitTrie target;
    target.Insert("target");
    target = std::move(source);
    check(target.Count() == 2, "move assignment restores count");
    check(target.GetValue(1) == "source", "move assignment restores value");
}

}  // namespace

int main() {
    test_insert_and_lookup();
    test_common_values();
    test_lock();
    test_save_load_and_move();

    std::cout << "\nPassed: " << passed << ", Failed: " << failed << '\n';
    return failed == 0 ? 0 : 1;
}
