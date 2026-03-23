#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Core/Constants.hpp" 

class RoundedRectangleShape : public sf::Shape {
private:
    sf::Vector2f size;
    float        radius;
    std::size_t  cornerPointCount;

public:
    explicit RoundedRectangleShape(
        sf::Vector2f size = {0.f, 0.f}, 
        float radius = Config::UI::DEFAULT_CORNER_RADIUS,
        std::size_t points = Config::UI::CORNER_POINTS);

    // Setters
    void setSize(const sf::Vector2f& size);
    void setRadius(float radius);
    void setCornerPointCount(std::size_t count);

    // Getters
    const sf::Vector2f& getSize() const;
    float getRadius() const;
    std::size_t getCornerPointCount() const;
    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};