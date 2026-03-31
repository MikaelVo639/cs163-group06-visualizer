#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "Core/AppContext.hpp"
#include "States/Screen.hpp"
#include "UI/Widgets/Button.hpp"
#include "UI/Widgets/InputBar.hpp"

class TestScreen : public Screen {
private:
    AppContext& ctx;

    UI::Widgets::InputBar inputBar;
    UI::Widgets::Button btnSubmit;

    sf::Text title;
    sf::Text resultText;

public:
    explicit TestScreen(AppContext& context);

    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw() override;
};