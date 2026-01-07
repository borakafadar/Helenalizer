//
// Created by borak on 26.12.2025.
//

#include "DrawObjects.h"

#include <complex>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "AudioProcessing.h"

constexpr float PI = 3.14159265358979f;

sf::VertexArray DrawObjects::getDoubleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY) {
    if (audio.empty()) {
        return sf::VertexArray(sf::PrimitiveType::LineStrip);
    }
    
    sf::VertexArray vertex(sf::PrimitiveType::TriangleStrip);

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

sf::VertexArray DrawObjects::getSingleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY) {
    if (audio.empty()) {
        return sf::VertexArray(sf::PrimitiveType::LineStrip);
    }

    sf::VertexArray vertex(sf::PrimitiveType::LineStrip);

    float centerY = windowSizeY / 2.0f;
    for (int i = 0; i < audio.size(); i+=2) {
        float x = (static_cast<float>(i) / audio.size()) * static_cast<float>(windowSizeX);
        float height = audio[i] * 300.0f;
        sf::Vertex topPoint;
        topPoint.position = sf::Vector2f(x,centerY - height);
        topPoint.color = topColor;

        vertex.append(topPoint);
    }

    return vertex;
}

//TODO:fix circle drawing
sf::VertexArray DrawObjects::getCircleAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY) {
    if (audio.empty()) {
        return sf::VertexArray(sf::PrimitiveType::LineStrip);
    }

    sf::VertexArray vertex(sf::PrimitiveType::LineStrip);

    float baseRadius = windowSizeX * 0.15f;
    int halfSize = audio.size() / 2;
    // We cover PI radians (180 degrees) for one side, then mirror
    float angleStep = PI / static_cast<float>(halfSize);

    // Right side (Calculated from bottom to top)
    for (int i = 0; i <= halfSize; i++) {
        int index = (i == halfSize) ? halfSize - 1 : i;
        float currentRadius = baseRadius + (audio[index] * 150.0f);
        // Start at PI/2 (90 degrees, bottom) and go around to -PI/2 (-90 degrees, top)
        float angle = (PI / 2.0f) - (i * angleStep);

        float x = windowSizeX / 2.0f + cos(angle) * currentRadius;
        float y = windowSizeY / 2.0f + sin(angle) * currentRadius;

        sf::Vertex point;
        point.position = sf::Vector2f(x, y);
        point.color = topColor;

        vertex.append(point);
    }

    // Left side (Mirroring the right side back down to the bottom)
    for (int i = halfSize; i >= 0; i--) {
        int index = (i == halfSize) ? halfSize - 1 : i;
        float currentRadius = baseRadius + (audio[index] * 150.0f);
        float angle = (PI / 2.0f) - (i * angleStep);

        // Mirror X relative to center to create the left half
        float x = windowSizeX / 2.0f - cos(angle) * currentRadius;
        float y = windowSizeY / 2.0f + sin(angle) * currentRadius;

        sf::Vertex point;
        point.position = sf::Vector2f(x, y);
        point.color = topColor;

        vertex.append(point);
    }

    return vertex;
}

sf::VertexArray DrawObjects::getSingleRainbowAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY) {
    if (audio.empty()) {
        return sf::VertexArray(sf::PrimitiveType::LineStrip);
    }

    sf::VertexArray vertex(sf::PrimitiveType::LineStrip);

    float centerY = windowSizeY / 2.0f;
    for (int i = 0; i < audio.size(); i+=2) {
        float x = (static_cast<float>(i) / audio.size()) * static_cast<float>(windowSizeX);
        float height = audio[i] * 300.0f;
        sf::Vertex topPoint;
        float ratio = static_cast<float>(i) / audio.size();
        auto red   = static_cast<uint8_t>(std::sin(ratio * 2 * PI) * 127 + 128);
        auto green = static_cast<uint8_t>(std::sin(ratio * 2 * PI + 2) * 127 + 128);
        auto blue  = static_cast<uint8_t>(std::sin(ratio * 2 * PI + 4) * 127 + 128);
        sf::Color color = {red,green,blue,255};
        topPoint.position = sf::Vector2f(x,centerY - height);
        topPoint.color = color;

        vertex.append(topPoint);
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



