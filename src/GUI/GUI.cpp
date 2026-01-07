#include "GUI.h"
#include <iostream>
#include <vector>
#include <optional>
#include <SFML/Graphics.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/AllWidgets.hpp>
#include <TGUI/Core.hpp>
#include <SFML/OpenGL.hpp>
#include <cmath>


#include "Button.h"
#include "../AudioProcessing.h"
#include "../DrawObjects.h"


constexpr int DEFAULT_WINDOW_X = 1920;
constexpr int DEFAULT_WINDOW_Y = 1080;

constexpr float DEFAULT_SCALE = .85f;


GUI::GUI() {
    renderWindow = nullptr;
    currentState = AppState::Visualizer;
    visualizerMode = VisualizerMode::Double;
    modelRotation = 0.0f;
    contextSettings = sf::ContextSettings();
}

GUI::~GUI() {
    if (renderWindow) {
        delete renderWindow;
    }
}

void GUI::setRenderWindow() {
    if (renderWindow == nullptr) {
        currentState = AppState::Visualizer;
        visualizerMode = VisualizerMode::Double;
        currentScale = DEFAULT_SCALE;
        contextSettings.antiAliasingLevel = 8;
        contextSettings.depthBits = 24;
        renderWindow = new sf::RenderWindow(sf::VideoMode({DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y}), "Helenalizer");
        renderWindow->setFramerateLimit(144);


        sf::Image icon;
        if (icon.loadFromFile("assets/wtfisthisphoto.png")) {
            renderWindow->setIcon(icon);
        }
    }
}



sf::RenderWindow &GUI::getRenderWindow() const {
    return *renderWindow;
}

void GUI::draw3DScene(float scale) {
    glClear(GL_DEPTH_BUFFER_BIT);
    renderWindow->pushGLStates();

    // --- 1. SETUP LIGHTING (The "Shading") ---
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // CRITICAL: Normalization
    // Since we are scaling the model, the normals get shrunk too.
    // This command tells OpenGL to fix them so lighting works correctly.
    glEnable(GL_NORMALIZE);

    // Define Light Properties (Sunlight)
    // Diffuse = The main color of the object when lit
    // Specular = The shiny white highlight (makes it look greasy/plastic)
    GLfloat lightAmbient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Bright white shine
    GLfloat lightPos[]      = { 50.0f, 50.0f, 50.0f, 1.0f }; // Top Right

    glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Define Material Properties (The Hotdog's surface)
    // Shininess: 0 = dull (cloth), 128 = very shiny (metal/wet)
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // --- 2. SETUP CAMERA ---
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float width = (float) renderWindow->getSize().x;
    float height = (float) renderWindow->getSize().y;
    float aspect = width / height;

    float zNear = 0.1f; float zFar = 100.0f; float fov = 45.0f;
    float fH = tan(fov * 3.14159f / 360.0f) * zNear;
    float fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // --- 3. DRAW FAKE SHADOW (The cheap trick) ---
    // We draw a flattened black circle underneath the model.
    glPushMatrix();
    glTranslatef(0.0f, -1.5f, -10.0f); // Move it slightly below the hotdog
    glScalef(1.0f, 0.1f, 1.0f);       // Flatten a sphere into a disk

    glDisable(GL_LIGHTING);           // Shadows don't need lighting
    glEnable(GL_BLEND);               // Enable transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // 50% transparent black

    // Draw a simple circle (using trigonometry)
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for(int i = 0; i <= 20; i++) {
        float angle = 2.0f * 3.14159f * float(i) / float(20);
        float dx = cos(angle) * 1.5f; // Radius 1.5
        float dy = sin(angle) * 1.5f;
        glVertex3f(dx, 0.0f, dy);
    }
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING); // Turn lights back on for the hotdog
    glPopMatrix();


    // --- 4. DRAW HOTDOG ---
    modelRotation += 0.5f;
    glColor3f(1.0f, 0.6f, 0.2f); // Hotdog Orange
    myModel.render(0.0f, 0.0f, -10.0f, scale, modelRotation);

    // 5. CLEANUP
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);


    glBindTexture(GL_TEXTURE_2D, 0);   // Unbind the Hotdog texture
    glDisable(GL_TEXTURE_2D);          // Turn off texturing completely

    renderWindow->popGLStates();
}


void GUI::run() {

    setRenderWindow();

    DrawObjects dob;

    sf::Texture tex;

    if (!tex.resize({DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y})) {
        std::cerr << "Failed to resize the texture!" << std::endl;
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


    //Loading the model
    if (!myModel.load("assets/hotdog.glb")) {
        std::cerr << "Warning: Could not load 3D model!" << std::endl;
    }


    //Initialize the pause menu things
    sf::Text text(font);
    text.setString("Paused");
    text.setCharacterSize(100);

    tgui::Gui menuGui(*renderWindow);

    auto topColorListBox = tgui::ListBox::create();
    std::vector<tgui::String> colors = {"White", "Blue", "Cyan", "Red"};
    topColorListBox->addMultipleItems(colors);
    //topColorListBox->setPosition("50% - width/2", "40%");

    topColorListBox->setSelectedItem("White");

    topColorListBox->onItemSelect([&dob](const tgui::String &item) {
        sf::Color selectedColor;
        if (item == "White") {
            selectedColor = sf::Color::White;
        } else if (item == "Blue") {
            selectedColor = sf::Color::Blue;
        } else if (item == "Cyan") {
            selectedColor = sf::Color::Cyan;
        } else if (item == "Red") {
            selectedColor = sf::Color::Red;
        }
        dob.setTopColor(selectedColor);
    });

    auto topColorLabel = tgui::Label::create("Top Color");
    topColorLabel->getRenderer()->setTextColor(sf::Color::White);
    topColorLabel->setTextSize(20);

    auto topColorLayout = tgui::VerticalLayout::create();

    topColorLayout->add(topColorLabel);
    topColorLayout->add(topColorListBox);
    topColorLayout->setPosition("50% - width/2", "40%");
    topColorLayout->setSize(150, 200);

    auto bottomColorListBox = tgui::ListBox::create();
    bottomColorListBox->addMultipleItems(colors);
    bottomColorListBox->setSelectedItem("Cyan");

    bottomColorListBox->onItemSelect([&dob](const tgui::String &item) {
        sf::Color selectedColor;
        if (item == "White") {
            selectedColor = sf::Color::White;
        } else if (item == "Blue") {
            selectedColor = sf::Color::Blue;
        } else if (item == "Cyan") {
            selectedColor = sf::Color::Cyan;
        } else if (item == "Red") {
            selectedColor = sf::Color::Red;
        }
        dob.setBottomColor(selectedColor);
    });

    auto bottomColorLabel = tgui::Label::create("Bottom Color");
    bottomColorLabel->getRenderer()->setTextColor(sf::Color::White);
    bottomColorLabel->setTextSize(20);

    auto bottomColorLayout = tgui::VerticalLayout::create();

    bottomColorLayout->add(bottomColorLabel);
    bottomColorLayout->add(bottomColorListBox);
    bottomColorLayout->setPosition("50% - width/2", "40%");
    bottomColorLayout->setSize(150, 200);


    auto colorsLayout = tgui::HorizontalLayout::create();
    colorsLayout->add(topColorLayout);
    colorsLayout->add(bottomColorLayout);

    auto exitButton = tgui::Button::create("Exit");

    exitButton->onPress([&menuGui]() {
        auto messageBox = tgui::MessageBox::create();
        messageBox->addButton("Yes");
        messageBox->addButton("No");
        messageBox->setButtonAlignment(tgui::HorizontalAlignment::Right);
        messageBox->setTitle("Exit");
        messageBox->setPosition("50% - width/2", "40%");
        messageBox->setText("Are you sure you want to exit?");
        menuGui.add(messageBox);
        messageBox->onButtonPress([msgBox=messageBox.get()](const tgui::String &button) {
            assert(button == "Yes" || button == "No");
            if (button == "Yes") {
                std::exit(0);
            } else if (button == "No") {
                msgBox->getParent()->remove(msgBox->shared_from_this());
            }
        });
    });
    exitButton->setPosition("50% - width/2", "90%");
    exitButton->setSize("10%", "5%");

    auto visualizerModeLayout = tgui::VerticalLayout::create();
    auto visualizerModeLabel = tgui::Label::create("Visualizer Mode");
    visualizerModeLabel->getRenderer()->setTextColor(sf::Color::White);
    visualizerModeLabel->setTextSize(20);

    std::vector<tgui::String> modes = {"Double", "Single", "Circle","Rainbow"};

    auto visualizerModeListBox = tgui::ListBox::create();
    visualizerModeListBox->addMultipleItems(modes);

    visualizerModeListBox->setSelectedItem("Double");
    visualizerModeListBox->onItemSelect([this](const tgui::String &item) {
        if (item == "Double") {
            visualizerMode = VisualizerMode::Double;
        } else if (item == "Single") {
            visualizerMode = VisualizerMode::Single;
        } else if (item == "Circle") {
            visualizerMode = VisualizerMode::Circle;
        } else if (item == "Rainbow") {
            visualizerMode = VisualizerMode::Rainbow;
        }
    });

    visualizerModeLayout->add(visualizerModeLabel);
    visualizerModeLayout->add(visualizerModeListBox);

    auto modelCheckBox = tgui::CheckBox::create();
    modelCheckBox->setChecked(false);
    modelCheckBox->setText("Show model");
    modelCheckBox->getRenderer()->setTextColor(sf::Color::White);
    bool modelChecked = false;
    modelCheckBox->onChange([&modelChecked](bool checked) {
        if (checked) {
            modelChecked = true;
        } else {
            modelChecked = false;
        }
    });

    auto menuLayout = tgui::VerticalLayout::create();
    menuLayout->add(colorsLayout);
    menuLayout->add(visualizerModeLayout);
    menuLayout->add(modelCheckBox);

    menuLayout->setSize("20%", "40%");
    menuLayout->setPosition("50% - width/2", "40%");




    menuGui.add(menuLayout);
    menuGui.add(exitButton);


    float btnWidth = 300.f;
    float btnHeight = 80.f;

    sf::FloatRect textPausedBounds = text.getLocalBounds();
    text.setPosition({
        (renderWindow->getSize().x - textPausedBounds.size.x) / 2,
        static_cast<float>(renderWindow->getSize().y / 100 * 5)
    });
    std::string fullscreenText("Toggle Fullscreen");
    Button fullscreenButton((renderWindow->getSize().x - textPausedBounds.size.x) / 2,
                            static_cast<float>((renderWindow->getSize().y / 100) * 25.0f), btnWidth, btnHeight,
                            "Toggle Fullscreen", font);


    AudioProcessing ap;
    ap.startRecording();


    bool fullscreenMode = false;

    while (renderWindow->isOpen()) {
        renderWindow->clear();

        renderWindow->resetGLStates();

        while (const std::optional event = renderWindow->pollEvent()) {
            if (currentState == AppState::Menu) {
                menuGui.handleEvent(*event);
            }
            if (event->is<sf::Event::Closed>()) {
                renderWindow->close();
            }
            if (event->is<sf::Event::KeyPressed>()) {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
                    if (currentState == AppState::Visualizer) {
                        currentState = AppState::Menu;
                    } else if (currentState == AppState::Menu) {
                        currentState = AppState::Visualizer;
                    }
                }
            }
        }

        sf::Vector2i pixelPos = sf::Mouse::getPosition(*renderWindow);
        sf::Vector2f mousePos = renderWindow->mapPixelToCoords(pixelPos);
        bool isClicking = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

        float x = renderWindow->getView().getSize().x;
        float y = renderWindow->getView().getSize().y;

        if (currentState == AppState::Visualizer) {
            std::vector<float> audioData = ap.getAudioData();

            renderWindow->draw(spr);

            sf::VertexArray vertexArray;

            switch (visualizerMode) {
                case VisualizerMode::Double:
                    vertexArray = dob.getDoubleAudioLine(audioData, x, y);
                    break;
                case VisualizerMode::Single:
                    vertexArray = dob.getSingleAudioLine(audioData, x, y);
                    break;
                case VisualizerMode::Circle:
                    vertexArray = dob.getCircleAudioLine(audioData, x, y);
                    break;
                case VisualizerMode::Rainbow:
                    vertexArray = dob.getSingleRainbowAudioLine(audioData,x,y);
                    break;
            }

            if (audioData.size() >= AudioProcessing::AUDIO_SIZE) {
                renderWindow->draw(vertexArray);
            }
            if (modelChecked) {
                float scale = DEFAULT_SCALE;

                if (!audioData.empty()) {
                    float bassEnergy = 0;

                    auto fftAudioData = ap.getFFTAudioData();
                    for (int i = 0; i < 5 && i < fftAudioData.size(); i++) {
                        bassEnergy += fftAudioData[i];
                    }
                    bassEnergy /= 5;

                    scale = DEFAULT_SCALE + (bassEnergy * 0.4f);
                }
                //for animation kinda behaviour
                currentScale += (scale - currentScale) * 0.1f;

                draw3DScene(currentScale);
            }

        } else if (currentState == AppState::Menu) {
            text.setPosition({(x - textPausedBounds.size.x) / 2.f, y / 100.f * 5.f});
            fullscreenButton.setPosition((x - fullscreenButton.getSize().x) / 2.0f, y / 100.f * 25.0f);


            menuGui.setWindow(*renderWindow);
            renderWindow->draw(text);
            menuGui.draw();
            fullscreenButton.draw(*renderWindow);
            fullscreenButton.update(mousePos);
            bool fullscreenButtonClicked = fullscreenButton.isPressed(mousePos, isClicking);
            if (fullscreenButtonClicked) {
                renderWindow->close();
                fullscreenMode = !fullscreenMode;
                if (fullscreenMode) {
                    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
                    renderWindow->create(sf::VideoMode({desktopMode.size.x, desktopMode.size.y}), "Helenalizer",
                                         sf::Style::Default, sf::State::Fullscreen, contextSettings);
                } else {
                    renderWindow->create(sf::VideoMode({DEFAULT_WINDOW_X, DEFAULT_WINDOW_Y}), "Helenalizer",
                                         sf::Style::Default, sf::State::Windowed, contextSettings);
                }
            }
        }


        renderWindow->display();
    }


    ap.stopRecording();
}
