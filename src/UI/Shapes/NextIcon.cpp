#include "UI/Shapes/NextIcon.hpp"

namespace UI::Shapes {
    NextIcon::NextIcon(float radius, float barWidth, float gap) {
        float triWidth = radius * 1.5f;
        float totalWidth = triWidth + gap + barWidth;

        triangle.setPointCount(3);
        triangle.setPoint(0, {0.f, -radius});
        triangle.setPoint(1, {triWidth, 0.f}); 
        triangle.setPoint(2, {0.f, radius});
        
        triangle.setOrigin({0.f, 0.f}); 
        triangle.setPosition({-totalWidth / 2.f, 0.f});
        triangle.setFillColor(sf::Color::White);

        bar.setSize({barWidth, radius * 2.f});
        bar.setOrigin({barWidth / 2.f, radius});
        
        bar.setPosition({totalWidth / 2.f - barWidth / 2.f, 0.f});
        bar.setFillColor(sf::Color::White);
    }

    void NextIcon::setFillColor(const sf::Color& color) {
        triangle.setFillColor(color);
        bar.setFillColor(color);
    }

    void NextIcon::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        target.draw(triangle, states);
        target.draw(bar, states);
    }
}