#include "States/HeapScreen.hpp"
#include <iostream>

HeapScreen::HeapScreen(AppContext& context)
    : DSAScreenBase(context),
      uiMenu(context),
      model(),
      controller(context, myGraph, model)
{
    myGraph.setDraggable(false);
}

void HeapScreen::handleEvent(const sf::Event& event) {
    uiMenu.handleEvent(event);
    DSAScreenBase::handleEvent(event);

    if (uiMenu.isBackClicked(event)) {
        ctx.nextState = ScreenState::MainMenu;
    }

    if (uiMenu.consumeGoClicked()) {
        if (myGraph.isAnimating()) {
            std::cout << "[WARNING] Wait!\n";
            return;
        }

        handleMenuAction();
        uiMenu.clearInputs();

        if (uiMenu.getActiveMenu() == UI::Widgets::ActiveMenu::Clean) {
            uiMenu.resetMenu();
        }
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            ctx.nextState = ScreenState::MainMenu;
        }
    }
}

void HeapScreen::handleMenuAction() {
    using namespace UI::Widgets;

    ActiveMenu menu = uiMenu.getActiveMenu();
    const auto& inputs = uiMenu.getInputs();

    if (menu == ActiveMenu::Create) {
        if (inputs.empty() || inputs[0].getText().empty()) return;
        int size = std::stoi(inputs[0].getText());
        controller.handleCreateRandom(size);
    }
    else if (menu == ActiveMenu::Insert) {
        if (inputs.empty() || inputs[0].getText().empty()) return;
        int val = std::stoi(inputs[0].getText());
        controller.handleInsert(val);
    }
    else if (menu == ActiveMenu::Remove) {
        if (inputs.empty() || inputs[0].getText().empty()) return;
        int val = std::stoi(inputs[0].getText());
        controller.handleRemove(val);
    }
    else if (menu == ActiveMenu::Search) {
        if (inputs.empty() || inputs[0].getText().empty()) return;
        int val = std::stoi(inputs[0].getText());
        controller.handleSearch(val);
    }
    else if (menu == ActiveMenu::Update) {
        if (inputs.size() < 2) return;
        if (inputs[0].getText().empty() || inputs[1].getText().empty()) return;

        int oldVal = std::stoi(inputs[0].getText());
        int newVal = std::stoi(inputs[1].getText());
        controller.handleUpdate(oldVal, newVal);
    }
    else if (menu == ActiveMenu::Clean) {
        controller.handleClearAll();
    }
}

void HeapScreen::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(ctx.window);
    uiMenu.update(mousePos);
    DSAScreenBase::update();
}

void HeapScreen::draw() {
    DSAScreenBase::draw();
    uiMenu.draw(ctx.window);
}