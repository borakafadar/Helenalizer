
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
int main() {

    sf::RenderWindow window (sf::VideoMode({1024,768}),"Helenalizer");
    sf::Texture tex("../src/wtfisthisphoto.jpg");
    sf::Sprite spr(tex);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            window.clear();
            window.draw(spr);
            spr.setPosition({100,100});
            window.display();
        }
    }
    return 0;
}