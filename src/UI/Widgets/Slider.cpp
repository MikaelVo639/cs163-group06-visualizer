#include "UI/Widgets/Slider.hpp"
#include <cmath>
#include <algorithm>

namespace UI::Widgets {

Slider::Slider(AppContext& context, sf::Vector2f pos, sf::Vector2f size) :
    ctx(context),
    track(size, size.y / 2.f),
    thumb({size.y * 1.8f, size.y * 1.8f}, (size.y * 1.8f) / 2.f),
    text(ctx.font, "50", Config::UI::FONT_SIZE_BUTTON),
    value(50.f)
{
    // Setup Track
    track.setPosition(pos);
    track.setFillColor(Config::UI::Colors::ButtonIdle);
    track.setOutlineThickness(Config::UI::BUTTON_OUTLINE);
    track.setOutlineColor(Config::UI::Colors::ButtonOutline);

    // Setup Thumb
    thumb.setFillColor(Config::UI::Colors::ButtonText); 
    thumb.setOutlineThickness(Config::UI::BUTTON_OUTLINE);
    thumb.setOutlineColor(Config::UI::Colors::ButtonOutlineHover);

    // Setup Text
    text.setFillColor(Config::UI::Colors::ButtonText);

    setValue(50.f); 
}

void Slider::update(sf::Vector2i mousePos) {
    sf::Vector2f mousePosF = static_cast<sf::Vector2f>(mousePos);

    isHovered = thumb.getGlobalBounds().contains(mousePosF) || 
                track.getGlobalBounds().contains(mousePosF);

    if (isDragging) {
        float mouseX = mousePosF.x;
        float trackX = track.getPosition().x;
        float trackWidth = track.getSize().x;
        
        // Calculate the percentage
        float percent = (mouseX - trackX) / trackWidth;
        percent = std::clamp(percent, 0.f, 1.f);
        
        setValue(percent * 100.f);
    }
    
    // Update thumb color based on stage
    if (isHovered || isDragging) {
        thumb.setFillColor(Config::UI::Colors::ButtonOutlineHover);
    } else {
        thumb.setFillColor(Config::UI::Colors::ButtonText);
    }
}

void Slider::handleEvent(const sf::Event& event) {
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left && isHovered) {
            isDragging = true;
        }
    }

    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            isDragging = false;
        }
    }
}

void Slider::setValue(float val) {
    value = std::clamp(val, 0.f, 100.f);
    updateVisuals();
}

float Slider::getValue() const {
    return value;
}

void Slider::updateVisuals() {
    text.setString(std::to_string(static_cast<int>(value)));

    float percent = value / 100.f;
    float trackX = track.getPosition().x;
    float trackY = track.getPosition().y;
    float trackWidth = track.getSize().x;
    float trackHeight = track.getSize().y;

    float thumbWidth = thumb.getSize().x;
    float thumbHeight = thumb.getSize().y;

    float centerX = trackX + (percent * trackWidth);
    float centerY = trackY + (trackHeight / 2.f);

    thumb.setPosition({centerX - thumbWidth / 2.f, centerY - thumbHeight / 2.f});
    text.setPosition({trackX + trackWidth + 20.f, trackY - (trackHeight / 2.f)});
}

void Slider::draw() {
    ctx.window.draw(track);
    ctx.window.draw(thumb);
    ctx.window.draw(text);
}

}