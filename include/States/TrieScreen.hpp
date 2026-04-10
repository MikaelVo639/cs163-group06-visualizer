#pragma once
#include "States/DSAScreenBase.hpp"
#include "UI/Components/TrieMenu.hpp"      // Your new UI
#include "Core/DSA/Trie.hpp"               // The future model
#include "Controllers/TrieController.hpp"  // The future controller

class TrieScreen : public DSAScreenBase {
private:
    UI::Widgets::TrieMenu uiMenu;

    // These represent the "Logic" layers someone else is working on
    Core::DSA::Trie model;
    Controllers::TrieController controller;

    // This handles interpreting the UI button clicks for the Trie
    void handleMenuAction();

public:
    explicit TrieScreen(AppContext& context);

    // Standard SFML / Screen lifecycle overrides
    void handleEvent(const sf::Event& event) override;
    void update() override;
    void draw() override;
};