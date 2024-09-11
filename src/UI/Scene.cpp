#include "Scene.h"
#include "../Config.h"
#include "../Utils/Math.h"

#include <utility>
#include <iostream>

const std::function<void(Scene*)>& Scene::getSceneChanger() const {
    return sceneChanger;
}

Scene::Scene(std::function<void(Scene*)> sceneChanger, sf::RenderWindow& window) : sceneChanger(std::move(sceneChanger)), window(window) {}

MenuScene::MenuScene(std::function<void(Scene *)> sceneChanger, sf::RenderWindow& window)
        : Scene(std::move(sceneChanger), window), layout({}) {
    float height = Config::Window::HEIGHT;
    float width = Config::Window::WIDTH;

    auto* label = new UI::Label("Minecraft Clone");
    label->setFontSize(Config::Widgets::TITLE_FONT_SIZE);
    label->setPosition({width / 2 - 370, 100});

    auto* startButton = new UI::Button("Play");
    startButton->setFontSize(Config::Widgets::FONT_SIZE);
    startButton->setSize({Config::Widgets::BUTTON_WIDTH, Config::Widgets::BUTTON_HEIGHT});
    startButton->setPosition({width / 2 - Config::Widgets::BUTTON_WIDTH / 2, height / 2 - 100});

    auto* exitButton = new UI::Button("Exit");
    exitButton->setFontSize(Config::Widgets::FONT_SIZE);
    exitButton->setSize({Config::Widgets::BUTTON_WIDTH, Config::Widgets::BUTTON_HEIGHT});
    exitButton->setPosition({width / 2 - Config::Widgets::BUTTON_WIDTH / 2, height / 2 + Config::Widgets::BUTTON_HEIGHT - 50});

    layout.addWidget(label);
    layout.addWidget(startButton);
    layout.addWidget(exitButton);
}

void MenuScene::update(float& deltaTime) {}

void MenuScene::render() const
{
    layout.render(window);
}

void MenuScene::onClick(sf::Vector2f position) {
    if (layout.getWidgets()[1]->isClicked(position)) {
        getSceneChanger()(new GameScene(getSceneChanger(), window));
    } else if (layout.getWidgets()[2]->isClicked(position)) {
        exit(0);
    }
}

void MenuScene::onResize(unsigned int width, unsigned int height) {}

GameScene::GameScene(std::function<void(Scene*)> sceneChanger, sf::RenderWindow& window) : Scene(std::move(sceneChanger), window), world() {
    // Setup OpenGL perspective matrix
    float fov = 45.f;
    float aspectRatio = static_cast<float>(Config::Window::WIDTH) / static_cast<float>(Config::Window::HEIGHT);
    float nearPlane = 0.1f;
    float farPlane = 100.f;

    // Assuming Math::setPerspectiveMatrix is a custom function for setting up projection matrices
    Math::setPerspectiveMatrix(fov, aspectRatio, nearPlane, farPlane);

    // Lock the mouse to the center of the window
    player.lockMouse(window);

    // World initialization
    world.init();
}

void GameScene::update(float& deltaTime) {
    player.update(deltaTime, window, world);  // Update the player based on input
}

void GameScene::render() const {
    // Apply the player transformations
    player.apply();

    // Render the world
    world.render(window);
}

void GameScene::onResize(unsigned int width, unsigned int height) {}

void GameScene::onClick(sf::Vector2f position) {
    player.lockMouse(window);
}
