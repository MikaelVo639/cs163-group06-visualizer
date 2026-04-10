#pragma once
#include <string>

namespace Core::DSA {

class Trie {
public:
    Trie() = default;

    // Dummy methods so the controller can call them
    void insert(const std::string& word) {}
    bool search(const std::string& word) { return false; }
    bool startsWith(const std::string& prefix) { return false; }
    void remove(const std::string& word) {}
    void clear() {}
};

}