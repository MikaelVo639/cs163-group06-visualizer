#pragma once

#include "Core/AppContext.hpp"
#include "Core/DSA/Heap.hpp"
#include "UI/DSA/Graph.hpp"

namespace Controllers {

class HeapController {
private:
    AppContext& ctx;
    UI::DSA::Graph& graph;
    Core::DSA::Heap& model;

    float centerX   = 800.f;
    float startY    = 320.f;
    float levelGap  = 120.f;
    float baseGap   = 90.f;

    void rebuildGraph();
    void layoutHeap();

public:
    HeapController(AppContext& context, UI::DSA::Graph& g, Core::DSA::Heap& m);

    void handleCreateRandom(int size);
    void handleInsert(int val);
    void handleRemove(int val);
    void handleSearch(int val);
    void handleUpdate(int oldVal, int newVal);
    void handleClearAll();
};

} // namespace Controllers