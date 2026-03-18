#include "double_array.h"

#include <iostream>
#include <algorithm>

int passed = 0;
int failed = 0;

void check(bool cond, const std::string& name) {
    if (cond) {
        passed++;
    } else {
        failed++;
        std::cout << "FAIL: " << name << std::endl;
    }
}

void test_getunit() {
    using namespace trie;

    std::vector<std::string> words = {
        "app", "apple", "application", "apply",
        "cat", "car", "card", "care",
        "dog", "door", "doors", "down",
        "he", "his", "she"
    };
    std::sort(words.begin(), words.end());

    DoubleArray<int> dat;
    dat.Build(words);

    for (size_t i = 0; i < words.size(); ++i) {
        auto r = dat.GetUnit(words[i]);
        check(r.found && r.value == static_cast<int>(i),
              "GetUnit(" + words[i] + ")");
    }

    std::vector<std::string> miss = {
        "a", "ap", "appl", "applications",
        "ca", "do", "dogs", "h", "sh", "", "xyz"
    };
    for (const auto& w : miss) {
        auto r = dat.GetUnit(w);
        check(!r.found, "GetUnit miss(" + w + ")");
    }
}

void test_getunit_with_values() {
    using namespace trie;

    std::vector<std::string> words = {"bar", "baz", "foo"};
    std::vector<int> values = {100, 200, 300};

    DoubleArray<int> dat;
    dat.Build(words, values);

    check(dat.GetUnit("foo").value == 300, "custom value foo=300");
    check(dat.GetUnit("bar").value == 100, "custom value bar=100");
    check(dat.GetUnit("baz").value == 200, "custom value baz=200");
}

void test_prefix_search() {
    using namespace trie;

    std::vector<std::string> words = {
        "a", "app", "apple", "application",
        "dog", "door", "doors"
    };
    std::sort(words.begin(), words.end());

    DoubleArray<int> dat;
    dat.Build(words);

    auto rs = dat.PrefixSearch("application");
    check(rs.size() == 3, "prefix count for 'application'");

    rs = dat.PrefixSearch("doors");
    check(rs.size() == 2, "prefix count for 'doors'");

    rs = dat.PrefixSearch("banana");
    check(rs.empty(), "prefix search 'banana' empty");

    rs = dat.PrefixSearch("application", 2);
    check(rs.size() == 2, "prefix search max_num=2");
}

void test_find_words_with_prefix() {
    using namespace trie;

    std::vector<std::string> words = {
        "app", "apple", "application", "apply", "approach",
        "car", "card", "care", "careful", "cat"
    };
    std::sort(words.begin(), words.end());

    DoubleArray<int> dat;
    dat.Build(words);

    auto rs = dat.FindWordsWithPrefix("app");
    check(rs.size() == 5, "find prefix 'app' count=5");

    rs = dat.FindWordsWithPrefix("car");
    check(rs.size() == 4, "find prefix 'car' count=4");

    rs = dat.FindWordsWithPrefix("xyz");
    check(rs.empty(), "find prefix 'xyz' empty");

    rs = dat.FindWordsWithPrefix("app", 2);
    check(rs.size() == 2, "find prefix 'app' max_num=2");
}

void test_empty_and_single() {
    using namespace trie;

    DoubleArray<int> dat;
    check(dat.Empty(), "empty before build");
    auto r = dat.GetUnit("foo");
    check(!r.found, "GetUnit on empty");

    std::vector<std::string> words = {"hello"};
    dat.Build(words);
    check(!dat.Empty(), "not empty after build");
    check(dat.GetUnit("hello").found, "single word found");
    check(!dat.GetUnit("hell").found, "single word prefix miss");
    check(!dat.GetUnit("helloo").found, "single word extended miss");
}

void test_overlapping_prefixes() {
    using namespace trie;

    std::vector<std::string> words = {
        "a", "ab", "abc", "abcd", "abcde"
    };

    DoubleArray<int> dat;
    dat.Build(words);

    for (size_t i = 0; i < words.size(); ++i) {
        auto r = dat.GetUnit(words[i]);
        check(r.found && r.value == static_cast<int>(i),
              "overlap GetUnit(" + words[i] + ")");
    }

    auto rs = dat.PrefixSearch("abcde");
    check(rs.size() == 5, "overlap prefix search count=5");

    rs = dat.FindWordsWithPrefix("a");
    check(rs.size() == 5, "overlap find prefix 'a' count=5");

    rs = dat.FindWordsWithPrefix("abc");
    check(rs.size() == 3, "overlap find prefix 'abc' count=3");
}

int main() {
    test_getunit();
    test_getunit_with_values();
    test_prefix_search();
    test_find_words_with_prefix();
    test_empty_and_single();
    test_overlapping_prefixes();

    std::cout << "\nPassed: " << passed << ", Failed: " << failed << std::endl;
    return failed > 0 ? 1 : 0;
}
