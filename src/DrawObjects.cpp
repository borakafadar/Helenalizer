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
    for (int i = 0; i < audio.size(); i+=1) {
        float x = (static_cast<float>(i) / audio.size()) * static_cast<float>(windowSizeX);
        float height = audio[i] * 250.0f;
        sf::Vertex topPoint;
        topPoint.position = sf::Vector2f(x,centerY - height);
        topPoint.color = sf::Color::White;

        sf::Vertex bottomPoint;
        bottomPoint.position = sf::Vector2f(x,centerY+height);
        bottomPoint.color= sf::Color::Cyan;

        vertex.append(topPoint);
        vertex.append(bottomPoint);
    }

    return vertex;
}
