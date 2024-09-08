#include "Headers/Game.h"
#include "Headers/Config.h"

Game::Game() : window(sf::VideoMode(Config::Window::WIDTH, Config::Window::HEIGHT), Config::Window::TITLE) {
    window.setFramerateLimit(Config::Window::FPS);

    this->currentScene = new MenuScene([this](Scene* scene) {
        this->currentScene = scene;
    });
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                    break;
                } case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        currentScene->onClick(window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}));
                    break;
                } case sf::Event::Resized: {
                    sf::FloatRect view(0, 0, event.size.width, event.size.height);
                    window.setView(sf::View(view));
                    currentScene->onResize(event.size.width, event.size.height);
                    break;
                }
                default:
                    break;
            }
        }

        this->currentScene->update(deltaTime);
        window.clear();
        this->currentScene->render(window);
        window.display();
    }
}