#include "UI/Components/HeapMenu.hpp"

namespace UI::Widgets {

HeapMenu::HeapMenu(AppContext& context)
    : DSAMenuBase(context, "Heap")
{
    updateLayout();
}

std::vector<std::string> HeapMenu::getMainButtonLabels() const {
    return {"Create", "Insert", "Delete", "Search", "Update", "Clear All"};
}

void HeapMenu::renderSubMenu(float boxX, float boxY, ActiveMenu type) {
    float innerX = boxX + 15.f;
    float innerY = boxY + 15.f;
    float boxHeight = 80.f;
    float boxWidth = 0.f;

    sf::Color innerBtnIdle  = Config::UI::Colors::ButtonIdle;
    sf::Color innerBtnHover = Config::UI::Colors::ButtonHover;
    sf::Color innerBtnPress = Config::UI::Colors::ButtonPressed;

    auto createInput = [&](const std::string& placeholder, float x, float w,
                           InputType inputType = InputType::AnyText) {
        activeInputs.emplace_back(ctx, sf::Vector2f{x, innerY}, sf::Vector2f{w, 45.f}, "", inputType);
        activeInputs.back().setPlaceholder(placeholder);
    };

    auto createExecuteBtn = [&](float x) {
        activeSubButtons.emplace_back(ctx, "Go", sf::Vector2f{x, innerY}, sf::Vector2f{90.f, 45.f});
        activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
    };

    float currentX = innerX;
    float gap = 15.f;

    if (type == ActiveMenu::Create) {
        createInput("Size", currentX, 120.f, InputType::Integer);
        currentX += 120.f + gap;
        createExecuteBtn(currentX);
        currentX += 90.f;
    }
    else if (type == ActiveMenu::Insert) {
        createInput("Value", currentX, 140.f, InputType::Integer);
        currentX += 140.f + gap;
        createExecuteBtn(currentX);
        currentX += 90.f;
    }
    else if (type == ActiveMenu::Remove) {
        createInput("Value", currentX, 140.f, InputType::Integer);
        currentX += 140.f + gap;
        createExecuteBtn(currentX);
        currentX += 90.f;
    }
    else if (type == ActiveMenu::Search) {
        createInput("Value", currentX, 140.f, InputType::Integer);
        currentX += 140.f + gap;
        createExecuteBtn(currentX);
        currentX += 90.f;
    }
    else if (type == ActiveMenu::Update) {
        createInput("Old Val", currentX, 150.f, InputType::Integer);
        currentX += 150.f + gap;
        createInput("New Val", currentX, 150.f, InputType::Integer);
        currentX += 150.f + gap;
        createExecuteBtn(currentX);
        currentX += 90.f;
    }

    boxWidth = (currentX - boxX) + 20.f;

    panelBg.setPosition({boxX, boxY});
    panelBg.setSize({boxWidth, boxHeight});

    float windowWidth = static_cast<float>(ctx.window.getSize().x);
    if (boxX + boxWidth > windowWidth - 20.f) {
        boxX = windowWidth - boxWidth - 20.f;
        panelBg.setPosition({boxX, boxY});
    }
}

} // namespace UI::Widgets