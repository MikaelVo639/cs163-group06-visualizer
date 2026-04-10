#include "States/TrieScreen.hpp"
#include <iostream>

// 1. The Constructor (You likely have this)
TrieScreen::TrieScreen(AppContext& context)
    : DSAScreenBase(context), 
      uiMenu(context),
      controller(context, myGraph, model) 
{
    myGraph.setDraggable(false);
}

// 2. The Event Handler (The vtable needs this!)
void TrieScreen::handleEvent(const sf::Event& event) {
    uiMenu.handleEvent(event);
    DSAScreenBase::handleEvent(event);

    if (uiMenu.isBackClicked(event)) {
        ctx.nextState = ScreenState::MainMenu;
    }

    if (uiMenu.consumeGoClicked()) {
        handleMenuAction();
    }
}

// 3. The Update Loop (The vtable needs this!)
void TrieScreen::update() {
    uiMenu.update(sf::Mouse::getPosition(ctx.window));
    DSAScreenBase::update();
}

// 4. The Draw Loop (The vtable needs this!)
void TrieScreen::draw() {
    DSAScreenBase::draw();
    uiMenu.draw(ctx.window);
}

// 5. Your logic handler
void TrieScreen::handleMenuAction() {
    std::cout << "Trie UI Action Triggered!" << std::endl;
}