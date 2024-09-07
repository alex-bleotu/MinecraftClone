#include "UserInterface.h"
#include "Config.h"

sf::Vector2f UI::Widget::getPosition() const {
    return position;
}

sf::Vector2f UI::Widget::getSize() const {
    return size;
}

void UI::Widget::setPosition(const sf::Vector2f position) {
    this->position = position;
}

void UI::Widget::setSize(const sf::Vector2f size) {
    this->size = size;
}

bool UI::Widget::isClicked(sf::Vector2f mousePosition) {
    return false;
}

UI::Button::Button(const std::string& text) {
    this->text = text;

    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(backgroundColor);

    border.setSize(size);
    border.setPosition(position);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(borderColor);
    border.setOutlineThickness(borderSize);

    buttonText.setString(text);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(textColor);
    buttonText.setFont(Config::Assets::get().font);
    buttonText.setPosition(position.x + size.x / 2 - buttonText.getGlobalBounds().width / 2, position.y + size.y / 2 - buttonText.getGlobalBounds().height / 2);
}

void UI::Button::update(float& deltaTime) {}

void UI::Button::render(sf::RenderWindow& window) const {
    window.draw(background);
    window.draw(border);
    window.draw(buttonText);
}

bool UI::Button::isClicked(const sf::Vector2f mousePosition) {
    if (mousePosition.x >= position.x && mousePosition.x <= position.x + size.x &&
        mousePosition.y >= position.y && mousePosition.y <= position.y + size.y)
        return true;
    return false;
}

void UI::Button::setPosition(const sf::Vector2f position) {
    this->position = position;

    background.setPosition(position);
    border.setPosition(position);
    buttonText.setPosition(position.x + size.x / 2 - buttonText.getGlobalBounds().width / 2, position.y + size.y / 2 - buttonText.getGlobalBounds().height / 2);
}

void UI::Button::setSize(const sf::Vector2f size) {
    this->size = size;

    background.setSize(size);
    border.setSize(size);
    buttonText.setPosition(position.x + size.x / 2 - buttonText.getGlobalBounds().width / 2, position.y + size.y / 2 - buttonText.getGlobalBounds().height / 2);
}

void UI::Button::setBackgroundColor(const sf::Color color) {
    backgroundColor = color;
    background.setFillColor(color);
}

void UI::Button::setBorderColor(const sf::Color color) {
    borderColor = color;
    border.setOutlineColor(color);
}

void UI::Button::setBorderSize(const float size) {
    borderSize = size;
    border.setOutlineThickness(size);
}

void UI::Button::setTextColor(const sf::Color color) {
    textColor = color;
    buttonText.setFillColor(color);
}

void UI::Button::setFontSize(const unsigned int size) {
    fontSize = size;
    buttonText.setCharacterSize(size);
}

void UI::Button::setText(const std::string text) {
    this->text = text;
    buttonText.setString(text);
    buttonText.setPosition(position.x + size.x / 2 - buttonText.getGlobalBounds().width / 2, position.y + size.y / 2 - buttonText.getGlobalBounds().height / 2);
}


UI::Label::Label(const std::string text) {
    this->text = text;

    labelText.setString(text);
    labelText.setCharacterSize(fontSize);
    labelText.setFillColor(textColor);
    labelText.setPosition(position);
    labelText.setFont(Config::Assets::get().font);
}

void UI::Label::update(float& deltaTime) {}

void UI::Label::render(sf::RenderWindow& window) const {
    window.draw(labelText);
}

void UI::Label::setPosition(const sf::Vector2f position) {
    this->position = position;

    labelText.setPosition(position);
}

void UI::Label::setSize(const sf::Vector2f size) {
    this->size = size;
}

void UI::Label::setTextColor(const sf::Color color) {
    textColor = color;
    labelText.setFillColor(color);
}

void UI::Label::setFontSize(const unsigned int size) {
    fontSize = size;
    labelText.setCharacterSize(size);
}

void UI::Label::setText(const std::string text) {
    this->text = text;
    labelText.setString(text);
}


UI::Layout::Layout(const std::vector<Widget*>& widgets, sf::Vector2f position) {
    this->widgets = widgets;
    this->position = position;

    for (auto widget : widgets) {
        widget->setPosition(widget->getPosition() + position);
    }
}

UI::Layout::~Layout() {
    for (auto widget : widgets) {
        delete widget;
    }
}

void UI::Layout::update(float& deltaTime) {
    for (auto widget : widgets) {
        widget->update(deltaTime);
    }
}

void UI::Layout::render(sf::RenderWindow& window) const {
    for (auto widget : widgets) {
        widget->render(window);
    }
}

void UI::Layout::addWidget(Widget* widget) {
    widgets.push_back(widget);
}

std::vector<UI::Widget*> UI::Layout::getWidgets() const {
    return widgets;
}