#include "trie.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <algorithm>

#include <stdint.h>

namespace trie {

CritbitTrie::~CritbitTrie() {
    if (data_.empty()) return;

    std::vector<Node*> s;
    s.push_back(root_);
    while (!s.empty()) {
        Node* n = s.back();
        s.pop_back();
        if (IsValue(n)) {
            delete NodeToValue(n);
            continue;
        }
        s.push_back(n->data[0]);
        s.push_back(n->data[1]);
        delete n;
    }
}

int GetZeroBit(uint8_t n) {
    uint8_t u = ~n;

    int p = 0;
    while (u) {
        if (u & 1) {
            return p;
        }
        u >>= 1;
        p++;
    }
    return p;
}

void CritbitTrie::Print() {
    print_(root_, 0);
}


void CritbitTrie::print_(Node* n, int s) {
    if (n == nullptr) return;
    std::string x(s, ' ');
    if(IsValue(n)) {
        auto v = NodeToValue(n);
        std::cout << x;
        std::cout << "Value: " << v->value << " " << v->i << std::endl;
        return;
    }
    std::cout << x;
    std::cout << "Node: " << n->pos << " " << GetZeroBit(n->byte) << std::endl;
    print_(n->data[0], s+1);
    print_(n->data[1], s+1);
}

void CritbitTrie::PrintCritbit(const std::string& value) {
    if (data_.empty()) {
        return;
    }

    std::vector<Node*> ps;

    Node* node = root_;
    while (!IsValue(node)) {
        const uint8_t c = node->pos < value.length() ? value[node->pos] : 0;
        const int s = ((c | node->byte) + 1) >> 8;
        std::cout << "pos:" << node->pos << " ";
        
        std::cout << "bit:" << GetZeroBit(node->byte) << " ";
        std::cout << "s:" << s << std::endl;
        ps.push_back(node);
        node = node->data[s];
    }

    for (auto p = ps.rbegin(); p != ps.rend(); p++) {
        if (IsValue((*p)->data[0])) {
            Value* v = NodeToValue((*p)->data[0]);
            std::cout << "Common " << v->value << std::endl;
        } else {
            break;
        }
    }

    Value* v = NodeToValue(node);
    std::cout << "v " << v->value << std::endl;

    return;
}

std::vector<std::string> CritbitTrie::GetCommonValues(const std::string& value) {
    std::vector<std::string> rs;
    if (data_.empty()) {
        return rs;
    }

    std::vector<Node*> ps;

    Node* node = root_;
    while (!IsValue(node)) {
        const uint8_t c = node->pos < value.length() ? value[node->pos] : 0;
        const int s = ((c | node->byte) + 1) >> 8;
        ps.push_back(node);
        node = node->data[s];
    }

    Value* v = NodeToValue(node);
    int n = v->value.size();
    if (value.substr(0, n) != v->value) {
        return rs;
    }
    rs.emplace_back(v->value);
    for (auto p = ps.rbegin(); p != ps.rend(); p++) {
        if (IsValue((*p)->data[0])) {
            Value* v = NodeToValue((*p)->data[0]);
            rs.emplace_back(v->value);
        } else {
            break;
        }
    }

    return rs;
}



std::vector<std::string> CritbitTrie::GetValues(const std::string& value) {
    std::vector<std::string> strs;
    std::vector<std::string> rs;

    if (data_.empty()) {
        return rs;
    }

    Node* node = root_;
    while (!IsValue(node) && node->pos < value.length()) {
        const uint8_t c = value[node->pos];
        const int s = ((c | node->byte) + 1) >> 8;
        node = node->data[s];
    }

    std::vector<Node*> s;
    Node* n = node;
    while (!IsValue(n) || !s.empty()) {
      while (!IsValue(n)) {
        s.push_back(n);
        n = n->data[0];
      }
      if (IsValue(n)) {
        strs.push_back(NodeToValue(n)->value);
      }
      n = s.back();
      s.pop_back();
      n = n->data[1];
    }
    if (IsValue(n)) {
        strs.push_back(NodeToValue(n)->value);
    }

    size_t size = value.size();
    for (auto& s : strs) {
        if (s.substr(0,size) == value) {
            rs.push_back(s);
        }
    }
    return rs;
}

void CritbitTrie::PrintValues() {
    std::vector<std::string> strs;
    if (data_.empty()) return;

    std::vector<Node*> s;
    Node* n = root_;
    while (!IsValue(n) || !s.empty()) {
      while (!IsValue(n)) {
        s.push_back(n);
        n = n->data[0];
      }
      if (IsValue(n)) {
        strs.push_back(NodeToValue(n)->value);
      }
      n = s.back();
      s.pop_back();
      n = n->data[1];
    }
    if (IsValue(n)) {
        strs.push_back(NodeToValue(n)->value);
    }

    for (auto& s : strs) {
      std::cout << s << " ";
    }
    std::cout << std::endl;

}

uint64_t CritbitTrie::Insert(const std::string& value) {
    // Handle empty tree case
    if (data_.empty()) {
        if (is_lock_) return -1;

        auto v = new Value(value, 0);
        data_.push_back(v);
        root_ = ValueToNode(data_.back());
        return 0;
    }

    // Search down the tree
    Node* node = root_;
    while (!IsValue(node)) {
        const uint8_t c = node->pos < value.length() ? value[node->pos] : 0;
        const int s = ((c | node->byte) + 1) >> 8;
        node = node->data[s];
    }

    // Compare with found value
    Value* e = NodeToValue(node);
    const std::string& t = e->value;

    size_t pos = 0;
    for (; pos < value.length() && pos < t.length(); pos++) {
        if (value[pos] != t[pos]) break;
    }

    uint8_t byte;
    if (pos != value.length()) { //  Prefix of v 
        byte = value[pos] ^ t[pos];
    } else if (pos < t.length()) { // v is Prefix of t
        byte = t[pos];
    } else { // v == t
        return e->i;
    }

    if (is_lock_) return -1;

    // Find critical bit
    while (byte & (byte-1)) {
        byte &= byte -1;
    }
    byte ^= 255;

    // Create new Value and internal Node
    const uint8_t c = t[pos];
    const int s = ((c | byte) + 1) >> 8;

    auto new_node = new Node();
    auto new_value = new Value(value, data_.size());

    new_node->pos = pos;
    new_node->byte = byte;
    new_node->data[1-s] = ValueToNode(new_value);

    // Insert new node in tree
    Node** tx = &root_;
    while (true) {
        Node* n = *tx;
        if (IsValue(n) || n->pos > pos) break;
        if (n->pos == pos && n->byte > byte) break;

        const uint8_t c = n->pos < value.length() ? value[n->pos] : 0;
        const int s = ((c | n->byte) + 1) >> 8;
        tx = &n->data[s];
    }

    new_node->data[s] = *tx;
    *tx = new_node;

    data_.push_back(new_value);
    return new_value->i;
}

const std::string& CritbitTrie::GetValue(uint64_t i) const {
    return data_[i]->value;
}

void CritbitTrie::Save(std::ostream& file) const {
    file << "#Trie# " << data_.size() << "\n";
    for (const auto& v : data_) {
        file << v->value << "\n";
    }
}


void CritbitTrie::Save(const std::string& filename) const {
    std::ofstream file(filename);
    Save(file);
}

void CritbitTrie::Load(std::istream& file) {
    uint64_t count;
    std::string str;

    file >> str >> count;

    file.ignore(); // Skip newline

    for (uint64_t i = 0; i < count; ++i) {
        std::string line;
        std::getline(file, line);
        Insert(line);
    }
}


void CritbitTrie::Load(const std::string &filename) {
    std::ifstream file(filename);
    Load(file);
}


std::unique_ptr<DoubleArrayTrie::Node> DoubleArrayTrie::BuildTrie(
        const std::vector<std::string>& strs) {
    auto root = std::make_unique<Node>();

    for (const auto& str : strs) {
        Node* current = root.get();

        for (char c : str) {
            auto it = std::find_if(current->value.begin(),
                                current->value.end(),
                                [c](const auto &pair) {
                                    return pair.first == c;
                                });
            if (it != current->value.end()) {
                current = it->second.get();
            } else {
                auto new_node = std::make_unique<Node>();
                Node* new_node_ptr = new_node.get();
                current->value.emplace_back(c, std::move(new_node));
                current = new_node_ptr;
            }
        }
        current->eow = true;
    }

    std::queue<Node*> que;
    que.push(root.get());

    while (!que.empty()) {
        Node* current = que.front();
        que.pop();

        std::sort(current->value.begin(), current->value.end(),
                  [](const auto& a, const auto& b) {
                    return a.first < b.first;
                  });

        for (auto& [_, next] : current->value) {
            if (next) {
                que.push(next.get());
            }
        }
    }

    return root;
}



int DoubleArrayTrie::GetValidBase(const Node* node) {
    if (node->value.empty()) return 0;

    int base = 1;
    while (true) {
        bool valid = true;
        for (const auto& [c, _] : node->value) {
            int code = Encode(c);
            int next = base + code;

            if (next >= static_cast<int>(check_.size())) {
                check_.resize(next + 1, -1);
                base_.resize(next + 1, 0);
                eow_.resize(next + 1, false);
            }

            if (check_[next] != -1) {
                valid = false;
                break;
            }
        }
        if (valid) break;
        base++;
    }
    return base;
}

void DoubleArrayTrie::Build(const std::vector<std::string>& strs) {
    auto root = BuildTrie(strs);

    std::queue<std::pair<int, Node*>> que;
    que.push({0, root.get()});
    // BFS， Level 
    while (!que.empty()) {
        auto [state, node] = que.front();
        que.pop();

        eow_[state] = node->eow;

        if (node->value.empty()) continue;

        int b = GetValidBase(node);
        base_[state] = b;

        for (const auto& [c, next_ptr] : node->value) {
            if (!next_ptr) continue;

            int code = Encode(c);
            int next = b + code;

            check_[next] = state;
            if (next >= static_cast<int>(base_.size())) {
                base_.resize(next+1, 0);
                eow_.resize(next+1, false);
            }

            que.push({next, next_ptr.get()});
        }
    }

}

bool DoubleArrayTrie::IsIn(const std::string& str) const {
    int state = 0;

    for (char c : str) {
        if (state >= static_cast<int>(base_.size())) return false;

        int code = Encode(c);
        int next = base_[state] + code;

        if (next < 0 || next >= static_cast<int>(check_.size()) || check_[next] != state) {
            return false;
        }

        state = next;
    }

    return state < static_cast<int>(eow_.size()) && eow_[state];
}

std::vector<std::string> DoubleArrayTrie::GetValues(const std::string& str) const {
    std::vector<std::string> rs;
    int state = 0;

    for (char c : str) {
        if (state >= static_cast<int>(base_.size())) return rs;

        int code = Encode(c);
        int next = base_[state] + code;

        if (next < 0 || next >= static_cast<int>(check_.size()) || check_[next] != state) {
            return rs;
        }

        state = next;
    }

    std::string current = str;
    Values(state, current, rs);  // DFS
    return rs;
}

void DoubleArrayTrie::Values(int state, std::string& current, 
                             std::vector<std::string>& rs) const {
    if (eow_[state]) {
        rs.push_back(current);
    }

    for (const auto& [next, c] : Nexts(state)) {
        current.push_back(c);
        Values(next, current, rs);
        current.pop_back();
    }
}

std::vector<std::pair<int, char>> DoubleArrayTrie::Nexts(int state) const {
    std::vector<std::pair<int, char>> nexts;
    if (state >= static_cast<int>(base_.size())) return nexts;

    int b = base_[state];
    for (int code = 1; code < 256; ++code) {
        int next = b + code;
        if (next < static_cast<int>(check_.size()) && check_[next] == state) {
            nexts.emplace_back(next, Decode(code));
        }
    }
    return nexts;
}

std::vector<std::string> DoubleArrayTrie::GetCommonValues(
    const std::string& str) const {
    std::vector<std::string> rs;
    int state = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        if (state >= static_cast<int>(base_.size())) break;

        int code = Encode(str[i]);
        int next = base_[state] + code;

        if (next < 0 || next >= static_cast<int>(check_.size()) || check_[next] != state) {
            break;
        }

        state = next;
        if (eow_[state]) {
            rs.push_back(str.substr(0, i+1));
        }
    }

    return rs;
}

} // namespace trie
