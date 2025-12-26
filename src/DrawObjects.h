//
// Created by borak on 26.12.2025.
//

#ifndef HELENALIZER_DRAWOBJECTS_H
#define HELENALIZER_DRAWOBJECTS_H
#include <SFML/Graphics/VertexArray.hpp>


class DrawObjects {
public:
    sf::VertexArray getAudioLine(std::vector<float> &audio, int windowSizeX, int windowSizeY);
private:
};


#endif //HELENALIZER_DRAWOBJECTS_H