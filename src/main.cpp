
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "AudioProcessing.h"

int main() {

    AudioProcessing ap;
    ap.getListeningDevices();
    ap.setListeningDevice();
    ap.startRecording();


    sf::RenderWindow window (sf::VideoMode({1024,768}),"Helenalizer");
    sf::Texture tex("../src/wtfisthisphoto.jpg");
    sf::Sprite spr(tex);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            ap.testRecording();
            window.clear();
            window.draw(spr);
            spr.setPosition({100,100});
            window.display();
        }
    }
    ap.stopRecording();
    return 0;
}