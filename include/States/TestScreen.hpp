#pragma once

#include "States/Screen.hpp"
#include "Core/AppContext.hpp"
#include "UI/Widgets/Button.hpp"
#include "UI/Widgets/InputBar.hpp"

#include <string>

class TestScreen : public Screen {
private:
    AppContext& ctx;

    UI::Widgets::InputBar inputBar;
    UI::Widgets::Button btnSubmit;

    sf::Text resultText;
    std::string lastInput;

    void updateResultText();

public:
    explicit TestScreen(AppContext& context);

    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw() override;
};