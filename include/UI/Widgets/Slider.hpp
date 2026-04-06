#pragma once
#include <SFML/Graphics.hpp>
#include <Core/AppContext.hpp>
#include <Core/Constants.hpp>
#include <UI/Shapes/RoundedRectangleShape.hpp>
#include <string>

namespace UI::Widgets {
class Slider {
private:
    AppContext&           ctx;
    sf::Text              text;
    RoundedRectangleShape track;
    RoundedRectangleShape thumb;

    float value;
    bool isHovered = false;
    bool isDragging = false;

public:
    // Context, position, size
    Slider(AppContext& context, sf::Vector2f pos, sf::Vector2f size);

    void update(sf::Vector2i mousePos);
    void handleEvent(const sf::Event& event);
    void draw();

    float getValue() const;
    void setValue(float val);

private:
    void updateVisuals();
};
}