#include "UI/Components/TrieMenu.hpp"

namespace UI::Widgets {

TrieMenu::TrieMenu(AppContext& context)
    : DSAMenuBase(context, "Trie")
{
    updateLayout();
}

std::vector<std::string> TrieMenu::getMainButtonLabels() const {
    return {"Create", "Insert", "Erase", "Search", "Clear All"};
}

void TrieMenu::renderSubMenu(float boxX, float boxY, ActiveMenu type) {
    // 1. Setup Coordinates and Padding
    float innerX = boxX + 15.f;
    float innerY = boxY + 15.f;
    float boxHeight = 80.f;
    float boxWidth = 0.f;

    // 2. Fetch Theme Colors from Config
    sf::Color innerBtnIdle   = Config::UI::Colors::ButtonIdle;
    sf::Color innerBtnHover  = Config::UI::Colors::ButtonHover;
    sf::Color innerBtnPress  = Config::UI::Colors::ButtonPressed;
    sf::Color dropdownHover  = Config::UI::Colors::ButtonHover;
    sf::Color dropdownPress  = Config::UI::Colors::ButtonPressed;

    // 3. Tool: Dropdown Creator (unchanged logic)
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

    // 4. Tool: Input Field Creator
    // NOTE: Defaulting to AnyText here is better for Tries than Integer!
    auto createInput = [&](const std::string& placeholder, float x, float w, InputType inputType = InputType::AnyText) {
        activeInputs.emplace_back(ctx, sf::Vector2f{x, innerY}, sf::Vector2f{w, 45.f}, "", inputType);
        activeInputs.back().setPlaceholder(placeholder);
    };

    // 5. Tool: Execution Button ("Go") Creator
    auto createExecuteBtn = [&](float x) {
        activeSubButtons.emplace_back(ctx, "Go", sf::Vector2f{x, innerY}, sf::Vector2f{90.f, 45.f});
        activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
    };

    float currentX = innerX;
    float gap = 15.f; 

    if (type == ActiveMenu::Create) {
        // 1. Change options to reflect Trie data (Sample words vs File)
        int sel = createDropdown({"Sample", "File"}, currentX, 160.f);
        currentX += 160.f + gap;

        if (sel == 0) { // Sample (Standard set of words like "app", "apple", "ball")
            // We don't necessarily need a "Size" input for a sample, 
            // but if you want to allow a custom number of random words:
            createInput("Count", currentX, 120.f, InputType::Integer);
            currentX += 120.f + gap;
            
            createExecuteBtn(currentX);
            currentX += 90.f;
        } 
        else if (sel == 1) { // File (Loading a dictionary file)
            // Keep the File logic the same as it's a standard feature
            activeSubButtons.emplace_back(ctx, "Edit", sf::Vector2f{currentX, innerY}, sf::Vector2f{90.f, 45.f});
            activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
            currentX += 90.f + gap;
            
            activeSubButtons.emplace_back(ctx, "Go", sf::Vector2f{currentX, innerY}, sf::Vector2f{90.f, 45.f});
            activeSubButtons.back().setColors(innerBtnIdle, innerBtnHover, innerBtnPress, sf::Color::White);
            currentX += 90.f;
        }
    }

    else if (type == ActiveMenu::Insert) {
        // 1. Create a single text input for the word
        createInput("Word", currentX, 200.f, InputType::Word);
        currentX += 200.f + gap;

        // 2. Add the Go button
        createExecuteBtn(currentX);
        currentX += 90.f;
    }

    else if (type == ActiveMenu::Remove) {
        createInput("Word", currentX, 200.f, InputType::Word);
        currentX += 200.f + gap;

        createExecuteBtn(currentX);
        currentX += 90.f;
    }

    else if (type == ActiveMenu::Search) {
        // 1. Let the user choose what kind of search they are doing
        int sel = createDropdown({"Word", "Prefix"}, currentX, 160.f);
        currentX += 160.f + gap;

        // 2. The input box for the search string
        createInput("Text", currentX, 160.f, InputType::AnyText);
        currentX += 160.f + gap;

        // 3. The Go button
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

}