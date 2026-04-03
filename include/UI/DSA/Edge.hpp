#pragma once
#include <SFML/Graphics.hpp>
#include "Core/AppContext.hpp"
#include "Core/Constants.hpp"
#include "Node.hpp"

namespace UI::DSA {

class Edge {
public:
    // 1. The Simplest: Just a basic connection
    Edge(Node* src, Node* dest, AppContext& context);

    // 2. Connection with Weight
    Edge(Node* src, Node* dest, AppContext& context, const std::string& weightStr);

    // 3. Connection with Weight and Directionality
    Edge(Node* src, Node* dest, AppContext& context, const std::string& weightStr, bool directed);

    // 4. The "Master": Everything included
    Edge(Node* src, Node* dest, AppContext& context, 
         const std::string& weightStr, bool directed, float thickness, sf::Color color);

    void update();

    void draw();

    // Setters for visual states (useful for DSA animations)
    void setColor(sf::Color color);
    void setThickness(float newThickness);

    void setWeight(std::string newWeight);
    std::string getWeight() const;

    void toggleDirection(bool directed);
    void flipDirection();
private:
    AppContext& ctx; // Reference to access the font

    Node* source;
    Node* dest;

    std::string weight;
    sf::Text weightText;

    sf::ConvexShape lineShape;
    sf::Color color;
    float thickness;

    sf::ConvexShape arrowhead;
    bool isDirected; // You can toggle this in the constructor
};

} 
// namespace UI::DSA