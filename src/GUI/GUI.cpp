#include "GUI.h"
#include <iostream>
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>


#include "Button.h"
#include "../AudioProcessing.h"
#include "../DrawObjects.h"


constexpr int WINDOW_X = 1920;
constexpr int WINDOW_Y = 1080;


GUI::GUI() {
    renderWindow = nullptr;
    currentState = AppState::Visualizer;
}

GUI::~GUI() {
    if (renderWindow) {
        delete renderWindow;
    }
}

void GUI::setRenderWindow() {
    if (renderWindow == nullptr) {
        currentState = AppState::Menu;
        renderWindow = new sf::RenderWindow(sf::VideoMode({WINDOW_X, WINDOW_Y}), "Helenalizer");
        renderWindow->setFramerateLimit(60);

        sf::Image icon;
        if (icon.loadFromFile("assets/wtfisthisphoto.png")) {
            renderWindow->setIcon(icon);
        }
    }
}

sf::RenderWindow& GUI::getRenderWindow() const {
    return *renderWindow;
}


void GUI::run() {

    setRenderWindow();

    sf::Texture tex;

    if (!tex.resize({WINDOW_X, WINDOW_Y})) {
        std::cerr << "Failed to resize the texture!" <<std::endl;
    }
    
    if (!tex.loadFromFile("assets/wtfisthisphoto.png")) {
        std::cerr << "Failed to load background image!" << std::endl;
    }
    
    sf::Sprite spr(tex);
    spr.setPosition({0, 0});


    sf::Font font;

     if (!font.openFromFile("assets/outfit.ttf")) {
         std::cerr << "Failed to load the font!" << std::endl;
     }

    //Initialize the pause menu things
    sf::Text text(font);
    text.setString("Paused");
    text.setCharacterSize(100);

    float btnWidth = 300.f;
    float btnHeight = 80.f;

    sf::FloatRect textPausedBounds = text.getLocalBounds();
    text.setPosition({(renderWindow->getSize().x - textPausedBounds.size.x)/2,static_cast<float>(renderWindow->getSize().y/100*5)});
    std::string fullscreenText("Toggle Fullscreen");
    Button fullscreenButton((renderWindow->getSize().x - textPausedBounds.size.x)/2,static_cast<float>((renderWindow->getSize().y/100)*25.0f),btnWidth,btnHeight,"Toggle Fullscreen",font);


    AudioProcessing ap;
    ap.startRecording();

    DrawObjects dob;

    bool fullscreenMode = false;

    while (renderWindow->isOpen()) {

        renderWindow->clear();

        while (const std::optional event = renderWindow->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                renderWindow->close();
            }
            if (event ->is<sf::Event::KeyPressed>()) {
                if (event -> getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
                    if (currentState == AppState::Visualizer) {
                        currentState = AppState::Menu;
                    } else if (currentState == AppState::Menu) {
                        currentState = AppState::Visualizer;
                    }
                }
            }
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(*renderWindow);
        sf::Vector2f mousePos = renderWindow -> mapPixelToCoords(pixelPos);
        bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        if (currentState == AppState::Visualizer) {
            std::vector<float> audioData = ap.getAudioData();

            renderWindow->draw(spr);


            if (audioData.size() >= 4096) {
                sf::VertexArray vertexArray = dob.getAudioLine(audioData, WINDOW_X, WINDOW_Y);
                renderWindow->draw(vertexArray);
            }
        } else if (currentState == AppState::Menu) {
            renderWindow->draw(text);
            fullscreenButton.draw(*renderWindow);
            fullscreenButton.update(mousePos);
            bool fullscreenButtonClicked = fullscreenButton.isPressed(mousePos,isClicking);
            if (fullscreenButtonClicked) {
                renderWindow -> close();
                fullscreenMode = !fullscreenMode;
                if (fullscreenMode) {
                    renderWindow -> create(sf::VideoMode({WINDOW_X,WINDOW_Y}),"Helenalizer",sf::Style::Default,sf::State::Fullscreen);

                } else {
                    renderWindow -> create(sf::VideoMode({WINDOW_X,WINDOW_Y}),"Helenalizer",sf::Style::Default,sf::State::Windowed);
                }
            }
        }


        renderWindow->display();
    }


    ap.stopRecording();
}