//
// Created by borak on 26.12.2025.
//

#ifndef HELENALIZER_DRAWOBJECTS_H
#define HELENALIZER_DRAWOBJECTS_H
#include <string>
#include <SFML/Graphics/VertexArray.hpp>


class DrawObjects {
public:
    sf::VertexArray getDoubleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY);
    sf::VertexArray getSingleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY);
    sf::VertexArray getCircleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY);
    sf::VertexArray getSingleRainbowAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY);
    void setTopColor(sf::Color newColor);
    void setBottomColor(sf::Color newColor);
    sf::Color getTopColor();
    sf::Color getBottomColor();


private:
    sf::Color topColor = sf::Color::White;
    sf::Color bottomColor = sf::Color::Cyan;
};


#endif //HELENALIZER_DRAWOBJECTS_H