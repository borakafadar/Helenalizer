//
// Created by borak on 26.12.2025.
//

#include "DrawObjects.h"
#include <SFML/Graphics.hpp>

//TODO add a return
sf::VertexArray DrawObjects::getAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY) {
    if (audio.size() < 4096) {
        return sf::VertexArray(sf::PrimitiveType::Lines);
    }
    sf::VertexArray vertex(sf::PrimitiveType::Lines);


    float centerY = windowSizeY / 2.0f;
    for (int i = 0; i < audio.size(); i+=2) {
        float x = (static_cast<float>(i) / audio.size()) * static_cast<float>(windowSizeX);
        float height = audio[i] * 300.0f;
        sf::Vertex topPoint;
        topPoint.position = sf::Vector2f(x,centerY - height);
        topPoint.color = topColor;

        sf::Vertex bottomPoint;
        bottomPoint.position = sf::Vector2f(x,centerY+height);
        bottomPoint.color= bottomColor;

        vertex.append(topPoint);
        vertex.append(bottomPoint);
    }

    return vertex;
}

void DrawObjects::setTopColor(sf::Color newColor) {
    topColor = newColor;
}

void DrawObjects::setBottomColor(sf::Color newColor) {
    bottomColor = newColor;
}

sf::Color DrawObjects::getTopColor() {
    return topColor;
}

sf::Color DrawObjects::getBottomColor() {
    return bottomColor;
}



