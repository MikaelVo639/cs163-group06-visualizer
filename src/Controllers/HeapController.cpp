#include "Controllers/HeapController.hpp"
#include "UI/Animations/Core/SequenceAnimation.hpp"
#include "UI/Animations/Core/CallbackAnimation.hpp"
#include "UI/Animations/Node/NodeColorAnimation.hpp"
#include "UI/Animations/Node/NodeScaleAnimation.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace Controllers {

HeapController::HeapController(AppContext& context, UI::DSA::Graph& g, Core::DSA::Heap& m)
    : ctx(context), graph(g), model(m) {}

void HeapController::layoutHeap() {
    const auto& pool = model.getPool();
    int n = static_cast<int>(pool.size());
    if (n == 0) return;

    int maxLevel = static_cast<int>(std::floor(std::log2(n)));

    for (int i = 0; i < n; ++i) {
        int level = static_cast<int>(std::floor(std::log2(i + 1)));
        int firstIndexInLevel = (1 << level) - 1;
        int posInLevel = i - firstIndexInLevel;
        int nodesInLevel = 1 << level;

        float spacing = baseGap * std::pow(2.f, static_cast<float>(maxLevel - level));
        float firstX = centerX - spacing * (nodesInLevel - 1) / 2.f;
        float x = firstX + posInLevel * spacing;
        float y = startY + level * levelGap;

        auto* node = graph.getNode(i);
        if (node) {
            node->setPosition({x, y});
        }
    }
}

void HeapController::rebuildGraph() {
    graph.clear();

    const auto& pool = model.getPool();
    for (int val : pool) {
        graph.addNode(std::to_string(val), {centerX, startY});
    }

    int n = static_cast<int>(pool.size());
    for (int i = 0; i < n; ++i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n)  graph.addEdge(i, left);
        if (right < n) graph.addEdge(i, right);
    }

    layoutHeap();
}

void HeapController::handleCreateRandom(int size) {
    if (size <= 0) return;

    model.clear();
    for (int i = 0; i < size; ++i) {
        model.insert(std::rand() % 100);
    }

    rebuildGraph();
}

void HeapController::handleInsert(int val) {
    model.insert(val);
    rebuildGraph();
}

void HeapController::handleRemove(int val) {
    if (!model.deleteValue(val)) {
        std::cout << "[UI LOG] Heap delete failed. Value not found: " << val << '\n';
        return;
    }

    rebuildGraph();
}

void HeapController::handleSearch(int val) {
    int foundIndex = model.search(val);
    if (foundIndex == -1) {
        std::cout << "[UI LOG] Heap search failed. Value not found: " << val << '\n';
        return;
    }

    auto sequence = std::make_unique<UI::Animations::SequenceAnimation>();

    for (int i = 0; i <= foundIndex; ++i) {
        auto* uiNode = graph.getNode(i);
        if (!uiNode) continue;

        sequence->add(std::make_unique<UI::Animations::NodeHighlightAnimation>(uiNode, 0.2f));

        if (i == foundIndex) {
            sequence->add(std::make_unique<UI::Animations::NodeScaleAnimation>(uiNode, 1.0f, 1.25f, 0.15f));
            sequence->add(std::make_unique<UI::Animations::NodeScaleAnimation>(uiNode, 1.25f, 1.0f, 0.15f));
            sequence->add(std::make_unique<UI::Animations::NodeUnhighlightAnimation>(uiNode, 0.2f));
        } else {
            sequence->add(std::make_unique<UI::Animations::NodeUnhighlightAnimation>(uiNode, 0.08f));
        }
    }

    ctx.animManager.addAnimation(std::move(sequence));
}

void HeapController::handleUpdate(int oldVal, int newVal) {
    int foundIndex = model.search(oldVal);
    if (foundIndex == -1) {
        std::cout << "[UI LOG] Heap update failed. Old value not found: " << oldVal << '\n';
        return;
    }

    auto sequence = std::make_unique<UI::Animations::SequenceAnimation>();

    for (int i = 0; i <= foundIndex; ++i) {
        auto* uiNode = graph.getNode(i);
        if (!uiNode) continue;

        sequence->add(std::make_unique<UI::Animations::NodeHighlightAnimation>(uiNode, 0.18f));

        if (i == foundIndex) {
            sequence->add(std::make_unique<UI::Animations::NodeScaleAnimation>(uiNode, 1.0f, 1.2f, 0.15f));
            sequence->add(std::make_unique<UI::Animations::CallbackAnimation>([this, oldVal, newVal]() {
                if (model.updateValue(oldVal, newVal)) {
                    rebuildGraph();
                }
            }));
            sequence->add(std::make_unique<UI::Animations::NodeScaleAnimation>(uiNode, 1.2f, 1.0f, 0.15f));
        } else {
            sequence->add(std::make_unique<UI::Animations::NodeUnhighlightAnimation>(uiNode, 0.08f));
        }
    }

    ctx.animManager.addAnimation(std::move(sequence));
}

void HeapController::handleClearAll() {
    model.clear();
    graph.clear();
}

} // namespace Controllers