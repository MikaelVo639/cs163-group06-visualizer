#pragma once
#include <SFML/Graphics.hpp>

namespace Config {
    namespace UI {
        //Default rounded rect shape
        inline constexpr float DEFAULT_CORNER_RADIUS = 10.f;
        inline constexpr std::size_t CORNER_POINTS = 15;
        inline constexpr float OUTLINE_THICKNESS = 2.f;
        
        // Font size
        inline constexpr unsigned int FONT_SIZE_BUTTON = 24;
        inline constexpr unsigned int FONT_SIZE_NODE = 20;
    
        //Button
        inline constexpr float BUTTON_WIDTH = 400.f;
        inline constexpr float BUTTON_HEIGHT = 70.f;
        inline constexpr float BUTTON_CORNER_RADIUS = 12.f;
        inline constexpr float BUTTON_OUTLINE = 2.f;
        
        namespace Colors {
            //Buttons
            const sf::Color ButtonIdle    = sf::Color(60, 60, 60);  
            const sf::Color ButtonHover   = sf::Color(85, 85, 85);  
            const sf::Color ButtonPressed = sf::Color(120, 120, 120); 
            const sf::Color ButtonOutline = sf::Color(100, 100, 100); 
            const sf::Color ButtonText    = sf::Color::White;
        }
    }

    namespace Colors {
        const sf::Color Background = sf::Color(30, 30, 30);
    }
}