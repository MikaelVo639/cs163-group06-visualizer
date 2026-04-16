#include "Controllers/TrieController.hpp"
#include "Core/DSA/PseudoCodeData.hpp"
#include "UI/DSA/LayoutEngine.hpp"
#include "UI/Animations/Core/AnimStepBuilder.hpp"
#include "UI/Animations/Core/SequenceAnimation.hpp"
#include "UI/Animations/Core/CallbackAnimation.hpp"
#include "UI/Animations/Node/NodeColorAnimation.hpp"
#include "UI/Animations/Node/NodeScaleAnimation.hpp"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

namespace Controllers {

    TrieController::TrieController(AppContext& context, UI::DSA::Graph& g, Core::DSA::Trie& m,
                                   UI::Widgets::PseudoCodeViewer* viewer)
        : ctx(context), graph(g), model(m), codeViewer(viewer) 
    {
        if (graph.getNodes().empty()) {
            graph.addNode("Root", {startX, startY});
            poolToGraphMap[model.getRootIndex()] = 0;
        }
    }

    void TrieController::syncGraph() {
        const auto& pool = model.getPool();
        
        struct NodeInfo { int poolIdx; char c; };
        std::vector<NodeInfo> stack = { {model.getRootIndex(), ' '} };
        std::unordered_set<int> currentActive;
        
        // DFS: Add Node and colors update
        while(!stack.empty()) {
            auto [pIdx, c] = stack.back(); stack.pop_back();
            currentActive.insert(pIdx);
            
            if (poolToGraphMap.find(pIdx) == poolToGraphMap.end()) {
                sf::Vector2f spawnPos = {startX, startY}; 
                std::string label = (c == ' ') ? "Root" : std::string(1, c);
                graph.addNode(label, spawnPos); 
                poolToGraphMap[pIdx] = graph.getNodes().size() - 1;
            }
            
            int gIdx = poolToGraphMap[pIdx];
            if (auto* n = graph.getNode(gIdx)) {
                if (pool[pIdx].isEndOfWord) {
                    n->setFillColor(sf::Color(70, 160, 100)); 
                } else {
                    n->setFillColor(Config::UI::Colors::NodeFill); 
                }
            }
            
            for(int i=25; i>=0; --i){
                if(pool[pIdx].children[i] != -1) {
                    stack.push_back({pool[pIdx].children[i], (char)('a' + i)});
                }
            }
        }
        
        std::vector<int> toDeletePidx;
        for(auto const& [pIdx, gIdx] : poolToGraphMap) {
            if (currentActive.find(pIdx) == currentActive.end()) {
                toDeletePidx.push_back(pIdx);
            }
        }
        
        if (!toDeletePidx.empty()) {
            std::vector<std::pair<int, int>> deleteList;
            for(int pIdx : toDeletePidx) deleteList.push_back({poolToGraphMap[pIdx], pIdx});
            
            std::sort(deleteList.rbegin(), deleteList.rend());
            
            for(auto const& [gIdxDeleted, pIdx] : deleteList) {
                graph.removeNodeAt(gIdxDeleted);
                poolToGraphMap.erase(pIdx);

                for(auto& [p, g] : poolToGraphMap) {
                    if (g > gIdxDeleted) g--;
                }
            }
        }
        
        graph.clearEdges();
        for(int pIdx : currentActive) {
            int srcGidx = poolToGraphMap[pIdx];
            for(int i=0; i<26; ++i) {
                int childPidx = pool[pIdx].children[i];
                if (childPidx != -1) {
                    int destGidx = poolToGraphMap[childPidx];
                    graph.addEdge(srcGidx, destGidx, "");
                }
            }
        }
    }

    void TrieController::triggerLayout(float duration) {
        auto layoutAnim = UI::DSA::LayoutEngine::asTrie(graph, model, poolToGraphMap, startX, startY, horizontalSpacing, verticalSpacing, duration);
        ctx.animManager.addAnimation(std::move(layoutAnim));
    }

    void TrieController::forceSnapLayout() {
        triggerLayout(0.f); 
    }

    // ==================== INSERT ====================
    void TrieController::handleInsert(const std::string& word) {
        using Builder = UI::Animations::AnimStepBuilder;
        auto codeDef = Core::DSA::PseudoCode::Trie::insert();
        Builder b(codeDef, codeViewer);

        // Count how many characters
        int tempCurr = model.getRootIndex();
        int existingChars = 0;
        for (char c : word) {
            int nextIdx = model.getPool()[tempCurr].children[c - 'a'];
            if (nextIdx != -1) {
                existingChars++;
                tempCurr = nextIdx;
            } else {
                break;
            }
        }

        // pre insert
        model.insert(word); 

        b.highlight("init_curr");
        int currPoolIdx = model.getRootIndex();
        
        for (int i = 0; i < word.length(); ++i) {
            char c = word[i];
            int charIndex = c - 'a';
            int nextPoolIdx = model.getPool()[currPoolIdx].children[charIndex];

            b.highlight("loop_char");
            
            b.callback([this, currPoolIdx]() {
                if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                    n->setFillColor(Config::UI::Colors::NodeHighlight); 
                    n->setLabelColor(Config::UI::Colors::LabelHighlight);
                }
            }).wait(0.2f);
            
            b.highlight("check_null");
            
            if (i >= existingChars) {
                b.highlight("create_node")
                 .callback([this, currPoolIdx, nextPoolIdx, c]() {
                     int parentUiIdx = poolToGraphMap[currPoolIdx];
                     sf::Vector2f parentPos = graph.getNode(parentUiIdx)->getPosition();
                     
                     graph.addNode(std::string(1, c), parentPos); 
                     int newUiIdx = graph.getNodes().size() - 1;
                     poolToGraphMap[nextPoolIdx] = newUiIdx;
                     
                     graph.addEdge(parentUiIdx, newUiIdx, "");
                     
                     triggerLayout();
                 })
                 .wait(0.4f);
            }
            
            b.highlight("advance")
             .callback([this, currPoolIdx]() {
                if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                    n->setLabelColor(Config::UI::Colors::NodeText);
                    if (model.getPool()[currPoolIdx].isEndOfWord) {
                        n->setFillColor(sf::Color(70, 160, 100)); 
                    } else {
                        n->setFillColor(Config::UI::Colors::NodeFill); 
                    }
                }
             }).wait(0.1f);
             
            currPoolIdx = nextPoolIdx;
        }

        b.highlight("set_end")
         .callback([this, currPoolIdx]() {
             if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                 n->setFillColor(sf::Color(70, 160, 100)); 
             }
             syncGraph(); 
             triggerLayout(0.2f);
         })
         .finish();

        ctx.animManager.addAnimation(b.build());
    }

    // ==================== SEARCH ====================
    void TrieController::handleSearch(const std::string& word, bool isPrefix) {
        using Builder = UI::Animations::AnimStepBuilder;
        auto codeDef = Core::DSA::PseudoCode::Trie::search();
        Builder b(codeDef, codeViewer);

        b.highlight("init_curr");

        int currPoolIdx = model.getRootIndex();
        bool broken = false;

        for (char c : word) {
            b.highlight("loop_char");
            
            int charIndex = c - 'a';
            int nextPoolIdx = model.getPool()[currPoolIdx].children[charIndex];
            
            int uiIdx = poolToGraphMap[currPoolIdx];
            auto* uiNode = graph.getNode(uiIdx);
            
            b.nodeHighlight(uiNode, 0.2f);

            b.highlight("check_null");
            if (nextPoolIdx == -1) {
                b.highlight("not_found");
                b.callback([this, currPoolIdx]() {
                    if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                        n->setLabelColor(Config::UI::Colors::NodeText); 
                        if (model.getPool()[currPoolIdx].isEndOfWord) n->setFillColor(sf::Color(70, 160, 100));
                        else n->setFillColor(Config::UI::Colors::NodeFill);
                    }
                }).wait(0.1f);
                
                broken = true;
                break;
            }

            b.highlight("advance")
             .callback([this, currPoolIdx]() {
                 if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                     n->setLabelColor(Config::UI::Colors::NodeText); 
                     if (model.getPool()[currPoolIdx].isEndOfWord) n->setFillColor(sf::Color(70, 160, 100));
                     else n->setFillColor(Config::UI::Colors::NodeFill);
                 }
             }).wait(0.1f);

            currPoolIdx = nextPoolIdx; 
        }

        if (!broken) {
            b.highlight("check_end");
            
            int finalUiIdx = poolToGraphMap[currPoolIdx];
            auto* finalNode = graph.getNode(finalUiIdx);

            if (!model.getPool()[currPoolIdx].isEndOfWord){
                b.nodeHighlight(finalNode, 0.2f);
            }

            if (model.getPool()[currPoolIdx].isEndOfWord || isPrefix) {
                b.highlight("found")
                 .nodeScale(finalNode, 1.0f, 1.3f, 0.2f)
                 .nodeScale(finalNode, 1.3f, 1.0f, 0.2f)
                 .callback([this, currPoolIdx]() {
                    if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                        n->setLabelColor(Config::UI::Colors::NodeText);
                        if (!model.getPool()[currPoolIdx].isEndOfWord){
                            n->setFillColor(Config::UI::Colors::NodeFill);
                        }
                    }
                 });
            } else {
                b.highlight("not_found_end")
                 .callback([this, currPoolIdx]() {
                    if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                        n->setLabelColor(Config::UI::Colors::NodeText);
                        n->setFillColor(Config::UI::Colors::NodeFill);
                    }
                 });
            }
        }

        b.finish();
        ctx.animManager.addAnimation(b.build());
    }

    // ==================== REMOVE ====================
    void TrieController::handleRemove(const std::string& word) {
        using Builder = UI::Animations::AnimStepBuilder;
        auto codeDef = Core::DSA::PseudoCode::Trie::deleteWord();
        Builder b(codeDef, codeViewer);

        b.highlight("init_curr"); 
        int currPoolIdx = model.getRootIndex();
        bool foundPath = true;
        
        for (char c : word) {
            b.highlight("loop_char"); 
            
            int charIndex = c - 'a';
            int nextPoolIdx = model.getPool()[currPoolIdx].children[charIndex];
            
            int uiIdx = poolToGraphMap[currPoolIdx];
            auto* uiNode = graph.getNode(uiIdx);
            
            b.nodeHighlight(uiNode, 0.2f); 
            
            b.highlight("check_null"); 
            if (nextPoolIdx == -1) {
                b.highlight("not_found"); 
                
                b.callback([this, currPoolIdx]() {
                    if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                        n->setLabelColor(Config::UI::Colors::NodeText); 
                        if (model.getPool()[currPoolIdx].isEndOfWord) n->setFillColor(sf::Color(70, 160, 100));
                        else n->setFillColor(Config::UI::Colors::NodeFill);
                    }
                }).wait(0.1f);
                
                foundPath = false;
                break;
            }
            
            b.highlight("advance"); 
            b.callback([this, currPoolIdx]() {
                if (auto* n = graph.getNode(poolToGraphMap[currPoolIdx])) {
                    n->setLabelColor(Config::UI::Colors::NodeText); 
                    if (model.getPool()[currPoolIdx].isEndOfWord) n->setFillColor(sf::Color(70, 160, 100));
                    else n->setFillColor(Config::UI::Colors::NodeFill);
                }
            }).wait(0.1f);
            
            currPoolIdx = nextPoolIdx;
        }

        if (foundPath && currPoolIdx != -1) {
            b.highlight("unmark_end"); 
            
            int finalUiIdx = poolToGraphMap[currPoolIdx];
            b.nodeHighlight(graph.getNode(finalUiIdx), 0.2f);
            b.nodeUnhighlight(graph.getNode(finalUiIdx), 0.2f);
        }

        b.highlight("delete") 
         .callback([this, word]() {
             bool existed = model.search(word); 
             
             model.deleteWord(word); 
             
             if (existed) { 
                 syncGraph();
                 triggerLayout();
             } else {
                 std::cout << "[UI LOG] Không thể xóa từ: " << word << " (Không tồn tại)\n";
             }
         })
         .finish();

        ctx.animManager.addAnimation(b.build());
    }
    
    // ==================== CLEAR ====================
    void TrieController::handleClearAll() {
        if (codeViewer) codeViewer->hide();

        if (graph.isAnimating()) {
            model.clear();
            graph.clear(); 
        }

        poolToGraphMap.clear();
        graph.clearEdges();
        int currentSize = graph.getNodes().size();
        for (int i = 0; i < currentSize; ++i) {
            graph.removeNodeAt(0); 
        }

        graph.addNode("Root", {startX, startY});
        poolToGraphMap[model.getRootIndex()] = 0; 
    }

} // namespace Controllers