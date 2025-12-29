#include "Button.h"

#include <iostream>

Button::Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font) : content(font,text,30) {
    // 1. Setup the Box (Shape)
    shape.setPosition({x, y});
    shape.setSize({width, height});

    // Define your colors here (Pink palette based on your image)
    idleColor = sf::Color(255, 182, 193); // Light Pink
    hoverColor = sf::Color(255, 105, 180); // Hot Pink
    activeColor = sf::Color(200, 50, 150); // Darker Pink

    shape.setFillColor(idleColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::Black);

    // 2. Setup the Text

    content.setFont(font);
    content.setString(text);
    content.setCharacterSize(30); // Adjust size as needed
    content.setFillColor(sf::Color::Black);

    // --- THE CENTERING MAGIC ---
    // Get the exact dimensions of the text
    sf::FloatRect textBounds = content.getLocalBounds();

    // Set origin to the textual center
    content.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    });

    // Move text to the geometrical center of the box
    content.setPosition({
        x + width / 2.0f,
        y + height / 2.0f
    });
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(content);
}

void Button::update(sf::Vector2f mousePos) {
    // Check if mouse is inside the box
    if (shape.getGlobalBounds().contains(mousePos)) {
        shape.setFillColor(hoverColor); // Highlight
    } else {
        shape.setFillColor(idleColor);  // Normal
    }
}

bool Button::isPressed(sf::Vector2f mousePos, bool isMousePressed) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        if (isMousePressed) {
            shape.setFillColor(activeColor); // Visual feedback
            return true;
        }
    }
    return false;
}