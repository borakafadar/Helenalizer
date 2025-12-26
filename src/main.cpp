#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <optional> // Required for std::optional

#include "AudioProcessing.h"
#include "DrawObjects.h"

int main() {
    constexpr int WINDOW_X = 1920;
    constexpr int WINDOW_Y = 1080;

    // 1. Create Window FIRST (Best practice)
    sf::RenderWindow window(sf::VideoMode({WINDOW_X, WINDOW_Y}), "Helenalizer");
    window.setFramerateLimit(60); // Important to prevent CPU melting

    // 2. Load Texture Safely
    sf::Texture tex;
    tex.resize({WINDOW_X,WINDOW_Y});

    tex.loadFromFile("assets/wtfisthisphoto.png");
    sf::Sprite spr(tex);
    spr.setPosition({0,0});


    // 3. Setup Audio
    AudioProcessing ap;
    ap.startRecording();

    DrawObjects dob;

    // --- THE MAIN LOOP ---
    while (window.isOpen()) {
        
        // A. EVENT POLLING (Handle inputs only)
        // ------------------------------------
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // B. UPDATE & DRAW (Run every frame)
        // ----------------------------------
        
        // 1. Get Data
        std::vector<float> audioData = ap.getAudioData();
        
        // 2. Clear Screen
        window.clear();

        // 3. Draw Background
        window.draw(spr);

        // 4. Draw Visualizer (Only if we have enough data)
        if (audioData.size() >= 4096) {
            sf::VertexArray vertexArray = dob.getAudioLine(audioData, WINDOW_X, WINDOW_Y);
            window.draw(vertexArray);
        }

        // 5. Display
        window.display();
    }

    ap.stopRecording();
    return 0;
}