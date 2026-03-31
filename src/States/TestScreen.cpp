#include "States/TestScreen.hpp"
#include "Core/Constants.hpp"

TestScreen::TestScreen(AppContext& context)
    : ctx(context),
      inputBar(context, {500.f, 300.f}, {400.f, 70.f}, "Enter something..."),
      btnSubmit(context, "SUBMIT", {500.f, 400.f}, {200.f, 60.f}),
      resultText(ctx.font, "", 28)
{
    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({500.f, 500.f});
}

void TestScreen::updateResultText() {
    resultText.setString("You entered: " + lastInput);
}

void TestScreen::handleEvent(const sf::Event& event) {
    inputBar.handleEvent(event);

    // Submit bằng button
    if (btnSubmit.isClicked(event)) {
        if (!inputBar.empty()) {
            lastInput = inputBar.getText();
            inputBar.clear();
            updateResultText();
        }
    }

    // Submit bằng Enter
    if (inputBar.isSubmitted(event)) {
        if (!inputBar.empty()) {
            lastInput = inputBar.getText();
            inputBar.clear();
            updateResultText();
        }
    }
}

void TestScreen::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(ctx.window);

    inputBar.update();
    btnSubmit.update(mousePos);
}

void TestScreen::draw() {
    inputBar.draw();
    btnSubmit.draw();
    ctx.window.draw(resultText);
}