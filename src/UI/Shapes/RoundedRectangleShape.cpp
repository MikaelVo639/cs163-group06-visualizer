#include "UI/Shapes/RoundedRectangleShape.hpp"

RoundedRectangleShape::RoundedRectangleShape(
    sf::Vector2f size, 
    float radius,
    std::size_t points)
    : size(size), radius(radius), cornerPointCount(points)
{
    update(); 
}

//setters
void RoundedRectangleShape::setSize(const sf::Vector2f& size) { 
    this->size = size; 
    update(); 
}
void RoundedRectangleShape::setRadius(float radius) { 
    this->radius = radius; 
    update(); 
}
void RoundedRectangleShape::setCornerPointCount(std::size_t count){
    this->cornerPointCount = count; 
    update(); 
}

// Getters
const sf::Vector2f& RoundedRectangleShape::getSize() const { 
    return size; 
}
float RoundedRectangleShape::getRadius() const { 
    return radius; 
}
std::size_t RoundedRectangleShape::getCornerPointCount() const { 
    return cornerPointCount; 
}

std::size_t RoundedRectangleShape::getPointCount() const {
    return cornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    if (index >= getPointCount()) return {0, 0};

    static const float PI = 3.141592654f;
    float deltaAngle = 90.f / static_cast<float>(cornerPointCount - 1);
    sf::Vector2f center;
    float angle = 0.f;

    unsigned int cornerIndex = static_cast<unsigned int>(index / cornerPointCount);
    
    switch (cornerIndex) {
        case 0: center = {size.x - radius, radius};          angle = 270.f; break;
        case 1: center = {size.x - radius, size.y - radius}; angle = 0.f;   break;
        case 2: center = {radius, size.y - radius};          angle = 90.f;  break;
        case 3: center = {radius, radius};                   angle = 180.f; break;
    }

    float localIndex = static_cast<float>(index % cornerPointCount);
    float rad = (angle + localIndex * deltaAngle) * PI / 180.f;
    
    return {center.x + std::cos(rad) * radius, center.y + std::sin(rad) * radius};
}