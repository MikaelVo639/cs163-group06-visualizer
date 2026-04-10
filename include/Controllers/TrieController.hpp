#pragma once
#include "Core/DSA/Trie.hpp"
#include "UI/DSA/Graph.hpp" // Adjust path based on your project structure
#include "Core/AppContext.hpp"

namespace Controllers {

class TrieController {
private:
    AppContext& ctx;
    UI::DSA::Graph& graph;
    Core::DSA::Trie& model;

public:
    // This constructor MUST match what you call in TrieScreen's initializer list
    TrieController(AppContext& context, UI::DSA::Graph& g, Core::DSA::Trie& m)
        : ctx(context), graph(g), model(m) {}

    // Dummy versions of the handlers called by handleMenuAction
    void handleInsert(const std::string& word) {}
    void handleSearch(const std::string& word, bool isPrefix) {}
    void handleRemove(const std::string& word) {}
    void handleClearAll() {
        model.clear();
        graph.clear();
    }
};

}