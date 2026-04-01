// Update sidebar 
// Now sidebar can validate the data u input from your keyboard
// 5 types of content:
// AnyText,
// Integer,
// IntegerList,
// Word,
// EdgeTriple
// Change the type u want to test: just look at the arrow below:

#include "States/TestScreen.hpp"
#include "Core/Constants.hpp"

TestScreen::TestScreen(AppContext& context)
    : ctx(context),
      inputBar(context,
               {150.f, 300.f},    //<----- 150.f is the minimum width, you can increase if u want
               {300.f, 70.f},
               "Enter your data:",
               UI::Widgets::InputType::AnyText),  //  <------------ this is where to change which type of data u input
      btnSubmit(context,
                "SUBMIT",
                {620.f, 400.f},
                {160.f, 60.f}),
      title(ctx.font, "INPUT BAR TEST", 36),
      resultText(ctx.font, "", 28)
{
    title.setFillColor(sf::Color::White);
    title.setPosition({560.f, 180.f});

    resultText.setFillColor(sf::Color::White);
    resultText.setPosition({500.f, 520.f});
}

void TestScreen::handleEvent(const sf::Event& event) {
    inputBar.handleEvent(event);

    if (btnSubmit.isClicked(event) || inputBar.isSubmitted(event)) {
        if (inputBar.empty()) {
            resultText.setString("Input is empty");
        }
        else if (!inputBar.valid()) {
            resultText.setString("Error: " + inputBar.getErrorMessage());
        }
        else {
            resultText.setString("Accepted: " + inputBar.getText());
            inputBar.clear();
        }
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            ctx.nextState = ScreenState::MainMenu;
        }
    }
}

void TestScreen::update() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(ctx.window);

    inputBar.update();
    btnSubmit.update(mousePos);
}

void TestScreen::draw() {
    ctx.window.draw(title);
    inputBar.draw();
    btnSubmit.draw();
    ctx.window.draw(resultText);
}