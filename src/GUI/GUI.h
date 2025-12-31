//
// Created by borak on 27.12.2025.
//

#ifndef HELENALIZER_GUI_H
#define HELENALIZER_GUI_H
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum class AppState {
    Visualizer,
    Menu
};


class GUI {
public:
    GUI();
    ~GUI();

    void setRenderWindow();
    [[nodiscard]] sf::RenderWindow& getRenderWindow() const;
    void run();


private:
    sf::RenderWindow* renderWindow;
    AppState currentState;
    sf::ContextSettings contextSettings;
};


#endif //HELENALIZER_GUI_H