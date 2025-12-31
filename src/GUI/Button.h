//
// Created by borak on 27.12.2025.
//

#ifndef HELENALIZER_BUTTON_H
#define HELENALIZER_BUTTON_H


#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    // Constructor: Needs position (x,y), size (w,h), label text, and the font
    Button(float x, float y, float width, float height, const std::string& text, const sf::Font& font);

    // Draws the button to the screen
    void draw(sf::RenderWindow& window);

    // Checks if the mouse is currently hovering over the button (changes color)
    void update(sf::Vector2f mousePos);

    // Checks if the button was actually clicked
    bool isPressed(sf::Vector2f mousePos, bool isMousePressed);
    
    // Updates the button's position
    void setPosition(float x, float y);

    // Gets the current position of the button
    sf::Vector2f getPosition() const;

    // Gets the current size of the button
    sf::Vector2f getSize() const;

private:
    sf::RectangleShape shape;
    sf::Text content;

    // Colors for interaction
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
};

#endif //HELENALIZER_BUTTON_H