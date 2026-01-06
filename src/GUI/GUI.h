//
// Created by borak on 27.12.2025.
//

#ifndef HELENALIZER_GUI_H
#define HELENALIZER_GUI_H
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Model3D.h"

enum class AppState {
    Visualizer,
    Menu
};

enum class VisualizerMode {
    Double,
    Single,
    Circle
};


class GUI {
public:
    GUI();
    ~GUI();

    void setRenderWindow();
    [[nodiscard]] sf::RenderWindow& getRenderWindow() const;
    void run();


private:

    void draw3DScene(float scale);

    Model3D myModel;
    float modelRotation = 0.0f;
    float currentScale = 1.0f;
    sf::RenderWindow* renderWindow;
    AppState currentState;
    VisualizerMode visualizerMode;
    sf::ContextSettings contextSettings;
};


#endif //HELENALIZER_GUI_H