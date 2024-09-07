#include "Scene.h"
#include "Config.h"

#include <utility>

const std::function<void(Scene*)>& Scene::getSceneChanger() const {
    return sceneChanger;
}

Scene::Scene(std::function<void(Scene*)> sceneChanger) : sceneChanger(std::move(sceneChanger)) {}

MenuScene::MenuScene(std::function<void(Scene *)> sceneChanger)
        : Scene(std::move(sceneChanger)), layout({}) {
    float height = Config::Window::HEIGHT;
    float width = Config::Window::WIDTH;

    auto* label = new UI::Label("Minecraft Clone");
    label->setFontSize(60);
    label->setPosition({width / 2 - 205, 100});

    auto* startButton = new UI::Button("Play");
    startButton->setFontSize(20);
    startButton->setSize({200, 50});
    startButton->setPosition({width / 2 - 100, height / 2});

    auto* exitButton = new UI::Button("Exit");
    exitButton->setFontSize(20);
    exitButton->setSize({200, 50});
    exitButton->setPosition({width / 2 - 100, height / 2 + 75});

    layout.addWidget(label);
    layout.addWidget(startButton);
    layout.addWidget(exitButton);
}

void MenuScene::update(float& deltaTime) {}

void MenuScene::render(sf::RenderWindow& window) const
{
    layout.render(window);
}

void MenuScene::onClick(sf::Vector2f position) {
    if (layout.getWidgets()[1]->isClicked(position)) {
        getSceneChanger()(new GameScene(getSceneChanger()));
    } else if (layout.getWidgets()[2]->isClicked(position)) {
        exit(0);
    }
}

void MenuScene::onResize(unsigned int width, unsigned int height) {}

GameScene::GameScene(std::function<void(Scene*)> sceneChanger) : Scene(std::move(sceneChanger)) {}

void GameScene::update(float& deltaTime) {}

void GameScene::render(sf::RenderWindow& window) const {
    window.clear(sf::Color::Green);
}

void GameScene::onResize(unsigned int width, unsigned int height) {}

void GameScene::onClick(sf::Vector2f position) {}
