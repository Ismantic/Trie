#pragma once 

#include <stdint.h>
#include <stdlib.h>

#include <vector>
#include <string>
#include <memory>
#include <iostream>

namespace trie {

class CritbitTrie {
private:
    struct Node {
        Node* data[2];
        uint32_t pos;
        uint8_t byte;
    };

    struct Value {
        std::string value;
        uint64_t i;

        Value(const std::string& v, uint64_t i) : value(v), i(i) {}
    };

    static Node* ValueToNode(Value* v) {
        return reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(v)|1);
    }
    static Value* NodeToValue(Node* n) {
        return reinterpret_cast<Value*>(reinterpret_cast<uintptr_t>(n) & ~1);
    }
    static bool IsValue(Node* n) {
        return reinterpret_cast<uintptr_t>(n) & 1;
    }

    Node* root_ = nullptr;
    std::vector<Value*> data_;
    bool is_lock_;

    void print_(Node* root, int s);

public:
    CritbitTrie() : root_(nullptr), is_lock_(false) {}
    ~CritbitTrie();

    CritbitTrie(const CritbitTrie&) = delete;
    CritbitTrie& operator=(const CritbitTrie&) = delete;

    CritbitTrie(CritbitTrie&& other) noexcept
        : root_(other.root_), data_(std::move(other.data_)), is_lock_(other.is_lock_) {
        other.root_ = nullptr;
        other.data_.clear();
    }
    CritbitTrie& operator=(CritbitTrie&& other) noexcept {
        if (this != &other) {
            this->~CritbitTrie();
            root_ = other.root_;
            data_ = std::move(other.data_);
            is_lock_ = other.is_lock_;
            other.root_ = nullptr;
            other.data_.clear();
        }
        return *this;
    }

    uint64_t Insert(const std::string& value);
    const std::string& GetValue(uint64_t i) const;

    void Save(const std::string& filename) const;
    void Load(const std::string& filename);

    void Save(std::ostream& file) const;
    void Load(std::istream& file);

    void PrintCritbit(const std::string& value);
    void Print();
    void PrintValues();

    std::vector<std::string> GetValues(const std::string& value);
    std::vector<std::string> GetCommonValues(const std::string& value);


    uint64_t Count() const { return data_.size(); }
    bool Lock(bool n) {
        bool o = is_lock_;
        is_lock_ = n;
        return o;
    }
    bool IsLock() const {
        return is_lock_;
    }

};

class DoubleArrayTrie {
private:
    int Encode(char c) const {
        return static_cast<int>(static_cast<uint8_t>(c)) + 1;
    }
    char Decode(int c) const {
        return static_cast<char>(static_cast<uint8_t>(c-1));
    }

    struct Node {
        std::vector<std::pair<char, std::unique_ptr<Node>>> value;
        bool eow; 
        Node() : eow(false) {}
    };
    
    std::unique_ptr<Node> BuildTrie(const std::vector<std::string>& strs);
    int GetValidBase(const Node* node);

    std::vector<int> base_;
    std::vector<int> check_;
    std::vector<bool> eow_;

    std::vector<std::pair<int, char>> Nexts(int state) const;
    void Values(int state, std::string& current, 
                std::vector<std::string>& rs) const;

public:
    DoubleArrayTrie() {
        base_.push_back(0);
        check_.push_back(-1);
        eow_.push_back(false);
    }

    void Build(const std::vector<std::string>& strs);

    bool IsIn(const std::string& str) const;

    std::vector<std::string> GetValues(const std::string& str) const;
    std::vector<std::string> GetCommonValues(const std::string& str) const;
    

};

} // namespace trie
