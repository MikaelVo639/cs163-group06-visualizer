#include "UI/Widgets/InputBar.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace UI::Widgets {

namespace {
    static std::string trim(const std::string& s) {
        std::size_t l = s.find_first_not_of(" \t\r\n");
        if (l == std::string::npos) return "";
        std::size_t r = s.find_last_not_of(" \t\r\n");
        return s.substr(l, r - l + 1);
    }
}

InputBar::InputBar(AppContext& context,
                   sf::Vector2f pos,
                   sf::Vector2f size,
                   const std::string& placeholder,
                   InputType inputType)
    : ctx(context),
      box(size, Config::UI::BUTTON_CORNER_RADIUS),
      text(ctx.font, "", Config::UI::FONT_SIZE_BUTTON),
      placeholderText(ctx.font, placeholder, Config::UI::FONT_SIZE_BUTTON),
      errorText(ctx.font, "", 18),
      idleColor(Config::UI::Colors::InputBarIdle),
      focusedColor(Config::UI::Colors::InputBarFocuse),
      invalidColor(sf::Color(55, 40, 40)),
      invalidOutlineColor(sf::Color::Red),
      errorColor(sf::Color::Red),
      outlineColor(Config::UI::Colors::InputBarOutline),
      focusedOutlineColor(Config::UI::Colors::InputBarFocusedOutline),
      textColor(Config::UI::Colors::InputBarText),
      placeholderColor(Config::UI::Colors::InputBarPlaceholder),
      type(inputType)
{
    box.setPosition(pos);
    box.setOutlineThickness(Config::UI::BUTTON_OUTLINE);
    box.setFillColor(idleColor);
    box.setOutlineColor(outlineColor);

    text.setFillColor(textColor);
    placeholderText.setFillColor(placeholderColor);
    errorText.setFillColor(errorColor);

    cursor.setSize({2.f, text.getCharacterSize() + 5.f});
    cursor.setFillColor(sf::Color::White);

    setType(inputType);
    updateTextPositions();
}

bool InputBar::isMultiline() const {
    return type == InputType::EdgeTriple;
}

bool InputBar::isCharacterAllowed(char32_t unicode) const {
    if (unicode == 13 || unicode == 10) {
        return isMultiline();
    }

    if (unicode < 32 || unicode > 126) return false;

    char c = static_cast<char>(unicode);

    switch (type) {
        case InputType::AnyText:
            return true;

        case InputType::Integer: {
            if (c == '-') return content.empty();

            if (!std::isdigit(static_cast<unsigned char>(c))) return false;

            if (content == "0" || content == "-0") return false;

            std::string candidate = content + c;
            if (candidate == "-") return true;

            try {
                std::size_t pos = 0;
                int value = std::stoi(candidate, &pos);
                if (pos != candidate.size()) return false;
                return value >= -999 && value <= 999;
            } catch (...) {
                return false;
            }
        }

        case InputType::IntegerList:
        case InputType::EdgeTriple:
            return std::isdigit(static_cast<unsigned char>(c)) || c == '-' || c == ' ';

        case InputType::Word:
            return std::isalpha(static_cast<unsigned char>(c));
    }

    return false;
}

bool InputBar::validateContent() {
    isValid = true;
    errorMessage.clear();

    if (content.empty()) {
        errorText.setString("");
        updateTextPositions();
        return true;
    }

    try {
        switch (type) {
            case InputType::AnyText:
                break;

            case InputType::Word: {
                if (content.size() > 20) {
                    isValid = false;
                    errorMessage = "Maximum 20 letters";
                    break;
                }
                for (char c : content) {
                    if (!std::isalpha(static_cast<unsigned char>(c))) {
                        isValid = false;
                        errorMessage = "Only letters are allowed";
                        break;
                    }
                }
                break;
            }

            case InputType::Integer: {
                std::size_t pos = 0;
                int value = std::stoi(content, &pos);

                if (pos != content.size()) {
                    isValid = false;
                    errorMessage = "Invalid integer";
                    break;
                }

                if (value < -999 || value > 999) {
                    isValid = false;
                    errorMessage = "Value must be from -999 to 999";
                }
                break;
            }

            case InputType::IntegerList: {
                std::stringstream ss(content);
                int x;
                bool hasNumber = false;

                while (ss >> x) {
                    hasNumber = true;
                    if (x < -999 || x > 999) {
                        isValid = false;
                        errorMessage = "Each value must be from -999 to 999";
                        break;
                    }
                }

                if (isValid && (!hasNumber || !ss.eof())) {
                    isValid = false;
                    errorMessage = "Enter integers separated by spaces";
                }
                break;
            }

            case InputType::EdgeTriple: {
                std::stringstream all(content);
                std::string line;
                int lineNo = 0;

                while (std::getline(all, line)) {
                    ++lineNo;
                    line = trim(line);
                    if (line.empty()) continue;

                    std::stringstream ss(line);
                    std::vector<int> nums;
                    int x;

                    while (ss >> x) {
                        if (x < -999 || x > 999) {
                            isValid = false;
                            errorMessage = "Line " + std::to_string(lineNo) + ": value must be from -999 to 999";
                            break;
                        }
                        nums.push_back(x);
                    }

                    if (!isValid) break;

                    if (!ss.eof()) {
                        isValid = false;
                        errorMessage = "Line " + std::to_string(lineNo) + ": only integers allowed";
                        break;
                    }

                    if (!(nums.size() == 1 || nums.size() == 3)) {
                        isValid = false;
                        errorMessage = "Line " + std::to_string(lineNo) + ": use 'x' or 'u v w'";
                        break;
                    }
                }
                break;
            }
        }
    }
    catch (...) {
        isValid = false;
        switch (type) {
            case InputType::Integer:
                errorMessage = "Invalid integer";
                break;
            case InputType::IntegerList:
                errorMessage = "Enter integers separated by spaces";
                break;
            case InputType::EdgeTriple:
                errorMessage = "Each line must be 'x' or 'u v w'";
                break;
            default:
                errorMessage = "Invalid input";
                break;
        }
    }

    errorText.setString(errorMessage);
    updateTextPositions();
    return isValid;
}

void InputBar::updateTextPositions() {
    sf::Vector2f pos  = box.getPosition();
    sf::Vector2f size = box.getSize();

    float paddingX = 15.f;
    float paddingY = 10.f;

    fitTextToBox(text);
    fitTextToBox(placeholderText);

    if (isMultiline()) {
        text.setOrigin({0.f, 0.f});
        placeholderText.setOrigin({0.f, 0.f});

        text.setPosition({pos.x + paddingX, pos.y + paddingY});
        placeholderText.setPosition({pos.x + paddingX, pos.y + paddingY});

        std::string lastLine;
        std::size_t lastBreak = content.find_last_of('\n');
        if (lastBreak == std::string::npos) lastLine = content;
        else lastLine = content.substr(lastBreak + 1);

        int lineCount = 1;
        for (char c : content) {
            if (c == '\n') ++lineCount;
        }

        sf::Text temp(ctx.font, lastLine, text.getCharacterSize());
        float lineHeight = static_cast<float>(text.getCharacterSize()) * 1.35f;

        float cursorX = pos.x + paddingX + temp.getLocalBounds().size.x + 2.f;
        float cursorY = pos.y + paddingY + (lineCount - 1) * lineHeight;

        if (content.empty()) {
            cursorX = pos.x + paddingX;
            cursorY = pos.y + paddingY;
        }

        cursor.setPosition({cursorX, cursorY});
        cursor.setSize({2.f, static_cast<float>(text.getCharacterSize()) + 5.f});
    } else {
        float centerY = pos.y + size.y / 2.f;

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({0.f, textBounds.position.y + textBounds.size.y / 2.f});
        text.setPosition({pos.x + paddingX, centerY});

        sf::FloatRect placeholderBounds = placeholderText.getLocalBounds();
        placeholderText.setOrigin({0.f, placeholderBounds.position.y + placeholderBounds.size.y / 2.f});
        placeholderText.setPosition({pos.x + paddingX, centerY});

        float cursorX;
        if (content.empty()) {
            cursorX = placeholderText.getPosition().x;
        } else {
            sf::FloatRect global = text.getGlobalBounds();
            cursorX = global.position.x + global.size.x + 2.f;
        }

        float cursorY = box.getPosition().y + box.getSize().y / 2.f - cursor.getSize().y / 2.f;
        cursor.setPosition({cursorX, cursorY});
        cursor.setSize({2.f, static_cast<float>(text.getCharacterSize()) + 5.f});
    }

    errorText.setPosition({pos.x + 4.f, pos.y + size.y + 4.f});
}

void InputBar::handleEvent(const sf::Event& event) {
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            sf::Vector2f mousePos(
                static_cast<float>(mousePressed->position.x),
                static_cast<float>(mousePressed->position.y)
            );

            isFocused = box.getGlobalBounds().contains(mousePos);

            if (isFocused) {
                cursorClock.restart();
                showCursor = true;
            }
        }
    }

    if (!isFocused) return;

    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
        char32_t unicode = textEntered->unicode;

        if (unicode == 8) {
            if (!content.empty()) {
                content.pop_back();
                text.setString(content);
                validateContent();
                updateTextPositions();
                cursorClock.restart();
                showCursor = true;
                dirty = true;
            }
            return;
        }

        if ((unicode == 13 || unicode == 10) && isMultiline()) {
            if (content.size() < maxLength) {
                content.push_back('\n');
                text.setString(content);
                validateContent();
                updateTextPositions();
                cursorClock.restart();
                showCursor = true;
                dirty = true;
            }
            return;
        }

        if (isCharacterAllowed(unicode)) {
            if (content.size() < maxLength) {
                content.push_back(static_cast<char>(unicode));
                text.setString(content);
                validateContent();
                updateTextPositions();
                cursorClock.restart();
                showCursor = true;
                dirty = true;
            }
        }
    }
}

void InputBar::update() {
    if (isFocused) {
        if (cursorClock.getElapsedTime().asSeconds() >= cursorBlinkTime) {
            showCursor = !showCursor;
            cursorClock.restart();
        }
    } else {
        showCursor = false;
    }

    if (!isValid) {
        box.setFillColor(invalidColor);
        box.setOutlineColor(invalidOutlineColor);
    } else if (isFocused) {
        box.setFillColor(focusedColor);
        box.setOutlineColor(focusedOutlineColor);
    } else {
        box.setFillColor(idleColor);
        box.setOutlineColor(outlineColor);
    }
}

void InputBar::draw() {
    ctx.window.draw(box);

    if (content.empty()) ctx.window.draw(placeholderText);
    else ctx.window.draw(text);

    if (isFocused && showCursor) {
        ctx.window.draw(cursor);
    }

    if (!errorMessage.empty()) {
        ctx.window.draw(errorText);
    }
}

void InputBar::setPosition(sf::Vector2f pos) {
    box.setPosition(pos);
    updateTextPositions();
}

void InputBar::setSize(sf::Vector2f size) {
    box.setSize(size);
    updateTextPositions();
}

void InputBar::setPlaceholder(const std::string& placeholder) {
    placeholderText.setString(placeholder);
    updateTextPositions();
}

void InputBar::setText(const std::string& value) {
    content = value;
    text.setString(content);
    validateContent();
    updateTextPositions();
    dirty = true;
}

void InputBar::setMaxLength(std::size_t length) {
    maxLength = length;
}

void InputBar::setFocus(bool focus) {
    isFocused = focus;
    if (isFocused) {
        cursorClock.restart();
        showCursor = true;
    }
}

void InputBar::setType(InputType inputType) {
    type = inputType;
    switch (type) {
        case InputType::Integer:
            maxLength = 4;
            break;
        case InputType::Word:
            maxLength = 20;
            break;
        case InputType::AnyText:
            maxLength = 50;
            break;
        case InputType::IntegerList:
            maxLength = 100;
            break;
        case InputType::EdgeTriple:
            maxLength = 2000;
            break;
    }
    validateContent();
}

void InputBar::fitTextToBox(sf::Text& target, float horizontalPadding, float verticalPadding) {
    sf::Vector2f size = box.getSize();

    unsigned int maxCharSize = Config::UI::FONT_SIZE_BUTTON;
    unsigned int minCharSize = 10;
    target.setCharacterSize(maxCharSize);

    while (target.getCharacterSize() > minCharSize) {
        sf::FloatRect bounds = target.getLocalBounds();

        float maxWidth = size.x - 2.f * horizontalPadding;
        float maxHeight = size.y - 2.f * verticalPadding;

        if (bounds.size.x <= maxWidth && bounds.size.y <= maxHeight) {
            break;
        }

        target.setCharacterSize(target.getCharacterSize() - 1);
    }
}

const std::string& InputBar::getText() const {
    return content;
}

bool InputBar::valid() const {
    return isValid;
}

const std::string& InputBar::getErrorMessage() const {
    return errorMessage;
}

bool InputBar::empty() const {
    return content.empty();
}

bool InputBar::focused() const {
    return isFocused;
}

void InputBar::clear() {
    content.clear();
    text.setString(content);
    errorMessage.clear();
    errorText.setString("");
    isValid = true;
    updateTextPositions();
    dirty = true;
}

bool InputBar::isSubmitted(const sf::Event& event) const {
    if (!isFocused) return false;
    if (isMultiline()) return false;

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        return keyPressed->code == sf::Keyboard::Key::Enter;
    }
    return false;
}

bool InputBar::consumeChanged() {
    if (dirty) {
        dirty = false;
        return true;
    }
    return false;
}

std::vector<std::string> InputBar::getLines(bool skipEmpty) const {
    std::vector<std::string> lines;
    std::stringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        line = trim(line);
        if (skipEmpty && line.empty()) continue;
        lines.push_back(line);
    }

    return lines;
}

bool InputBar::parseGraphData(int nodeCount,
                              std::vector<int>& nodeValues,
                              std::vector<std::tuple<int, int, int>>& edges,
                              std::string& outError) const {
    nodeValues.clear();
    edges.clear();
    outError.clear();

    auto lines = getLines(true);

    if (static_cast<int>(lines.size()) < nodeCount) {
        outError = "Not enough node lines";
        return false;
    }

    for (int i = 0; i < nodeCount; ++i) {
        std::stringstream ss(lines[i]);
        int x;
        char extra;

        if (!(ss >> x) || (ss >> extra)) {
            outError = "Node line " + std::to_string(i + 1) + " must contain exactly 1 integer";
            return false;
        }

        nodeValues.push_back(x);
    }

    for (std::size_t i = nodeCount; i < lines.size(); ++i) {
        std::stringstream ss(lines[i]);
        int u, v, w;
        char extra;

        if (!(ss >> u >> v >> w) || (ss >> extra)) {
            outError = "Edge line " + std::to_string(i + 1) + " must be: u v w";
            return false;
        }

        edges.emplace_back(u, v, w);
    }

    return true;
}

}