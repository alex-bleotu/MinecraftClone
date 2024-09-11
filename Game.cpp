#include "Game.h"
#include "src/Config.h"
#include "src/Utils/Math.h"
#include "src/Utils/Texture.h"

Game::Game() {
    sf::ContextSettings settings;
    settings.depthBits = Config::Window::DEPTH_BITS; // Depth buffer
    window.create(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), Config::Window::TITLE, sf::Style::Default, settings);
    window.setFramerateLimit(Config::Window::FPS);

    this->currentScene = new MenuScene([this](Scene* scene) {
        this->currentScene = scene;
    }, window);
}

void Game::run() {
    Texture::loadTextures();

    // Main game loop
    sf::Clock clock;

    running = true;

    while (window.isOpen()) {
        window.clear(sf::Color::Black);

        float deltaTime = clock.restart().asSeconds();

        sf::Event event{};

        // Process events (SFML still handles the window and input)
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                }
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        currentScene->onClick(window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}));
                    break;
                }
                case sf::Event::Resized: {
                    currentScene->onResize(event.size.width, event.size.height);
                    break;
                }
                default:
                    break;
            }
        }

        // Update game state
        currentScene->update(deltaTime);

        // Render your current scene (OpenGL rendering inside the scene)
        currentScene->render();

        // Swap the buffers and display the rendered frame
        window.display();
    }
}