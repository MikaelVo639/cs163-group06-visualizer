#include "UI/Components/HeapMenu.hpp"

namespace UI::Widgets {

HeapMenu::HeapMenu(AppContext& context)
    : DSAMenuBase(context, "Heap")
{
    updateLayout();
}

std::vector<std::string> HeapMenu::getMainButtonLabels() const {
    // Indices: 0: Create, 1: Insert, 2: Delete Root, 3: Find Maximum, 4: Clear All
    return {"Create", "Insert", "Delete Root", "Find Maximum", "Clear All"};
}

bool HeapMenu::isInstantAction(int index) const {
    // Index 4 maps to the "Clear All" action
    return index == 4;
}

void HeapMenu::setMainButtonEnabled(int index, bool enabled) {
    // Simply call the base class function. 
    // It will handle the mainButtonsEnabled vector AND the grey-out colors.
    DSAMenuBase::setMainButtonEnabled(index, enabled);
}

void HeapMenu::updateLayout() {
    DSAMenuBase::updateLayout();

    if (mainButtons.empty()) return;

    sf::Vector2f startPos = mainButtons[0].getPosition();
    float btnWidth = 200.f; 
    float btnHeight = 45.f;
    float gap = 12.f;

    for (int i = 0; i < (int)mainButtons.size(); ++i) {
        mainButtons[i].setSize({btnWidth, btnHeight});
        mainButtons[i].setPosition({startPos.x + i * (btnWidth + gap), startPos.y});
    }
}

void HeapMenu::renderSubMenu(float boxX, float boxY, int menuIndex) {
    float innerX = boxX + 15.f;
    float innerY = boxY + 15.f;
    float boxHeight = 80.f;
    float boxWidth = 0.f;

    sf::Color innerBtnIdle = Config::UI::Colors::ButtonIdle;
    sf::Color innerBtnHover = Config::UI::Colors::ButtonHover;
    sf::Color innerBtnPress = Config::UI::Colors::ButtonPressed;
    sf::Color dropdownHover = Config::UI::Colors::ButtonHover;
    sf::Color dropdownPress = Config::UI::Colors::ButtonPressed;

    auto createDropdown = [&](const std::vector<std::string>& options, float x, float w) {
        dropdownAction.emplace(ctx, "Select...", sf::Vector2f{x, innerY}, sf::Vector2f{w, 45.f});
        dropdownAction->setColors(innerBtnIdle, dropdownHover, dropdownPress, sf::Color::White);
        dropdownAction->setOptions(options);
        
        if (lastDropdownIndex >= 0 && lastDropdownIndex < static_cast<int>(options.size())) {
            dropdownAction->setSelectedIndex(lastDropdownIndex);
        } else {
            dropdownAction->setSelectedIndex(0);
            lastDropdownIndex = 0;
        }

        dropdownAction->setLabel(dropdownAction->getSelectedText());
        return dropdownAction->getSelectedIndex();
    };

    auto createInput = [&](const std::string& placeholder, float x, float w, InputType inputType = InputType::AnyText) {
        activeInputs.emplace_back(ctx, sf::Vector2f{x, innerY}, sf::Vector2f{w, 45.f}, "", inputType);
        activeInputs.back().setPlaceholder(placeholder);
    };

    auto createExecuteBtn = [&](const std::string& label, float x) {
        activeSubButtons.emplace_back(ctx, label, sf::Vector2f{x, innerY}, sf::Vector2f{90.f, 45.f});
        activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
    };

    float currentX = innerX;
    float gap = 15.f; 

    // Logic follows the getMainButtonLabels indices
    if (menuIndex == 0) { // Create
        int sel = createDropdown({"Random", "File"}, currentX, 160.f);
        currentX += 160.f + gap;

        if (sel == 0) { // Random
            createInput("Size", currentX, 120.f, InputType::Integer);
            currentX += 120.f + gap;
            createExecuteBtn("Create", currentX);
            currentX += 90.f + gap;
            createExecuteBtn("Heapify", currentX);
            currentX += 90.f;
        } else if (sel == 1) { // File
            activeSubButtons.emplace_back(ctx, "Edit", sf::Vector2f{currentX, innerY}, sf::Vector2f{90.f, 45.f});
            activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
            currentX += 90.f + gap;
            
            createExecuteBtn("Create", currentX);
            currentX += 90.f + gap;

            createExecuteBtn("Heapify", currentX);
            currentX += 90.f;
        }
    }
    else if (menuIndex == 1) { // Insert
        createInput("Value", currentX, 140.f, InputType::Integer);
        currentX += 140.f + gap;
        createExecuteBtn("Insert", currentX);
        currentX += 90.f;
    }
    else if (menuIndex == 2 || menuIndex == 3) { // Delete Root or Find Maximum
        createExecuteBtn("Go", currentX);
        currentX += 90.f;
    }
    else if (menuIndex == 4) { // Clear All
        createExecuteBtn("Go", currentX);
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