#pragma once
#include <SFML/Graphics.hpp>

namespace UI::Shapes {
    class PrevIcon : public sf::Drawable, public sf::Transformable {
    private:
        sf::ConvexShape triangle;
        sf::RectangleShape bar;
    public:
        PrevIcon(float radius = 8.f, float barWidth = 3.f, float gap = 4.f);
        void setFillColor(const sf::Color& color);
    protected:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };
}