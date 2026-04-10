#pragma once

#include "Core/AppContext.hpp"
#include "Core/DSA/Heap.hpp"
#include "UI/DSA/Graph.hpp"
#include <string>

namespace Controllers {

    class HeapController {
    private:
        AppContext& ctx;
        UI::DSA::Graph& graph;
        Core::DSA::Heap& model;

        float centerX  = 800.f;
        float startY   = 260.f;
        float levelGap = 120.f;
        float baseGap  = 90.f;

        bool pendingRebuild = false;

        void syncGraphEdges();
        void triggerLayout(float duration = 0.5f);
        void rebuildGraphFromModel();

    public:
        HeapController(AppContext& context, UI::DSA::Graph& g, Core::DSA::Heap& m);

        void processDeferredActions();

        void handleCreateRandom(int size);
        void handleCreateFromFile();
        void handleEditDataFile();
        void handleInsert(int val);
        void handleRemove(int val);
        void handleSearch(int targetValue);
        void handleUpdate(int oldVal, int newVal);
        void handleClearAll();
    };

}