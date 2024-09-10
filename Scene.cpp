#include "Headers/Scene.h"
#include "Headers/Config.h"

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

GameScene::GameScene(std::function<void(Scene*)> sceneChanger) : Scene(std::move(sceneChanger)) {
    cubes.emplace_back(200);
    cubes.emplace_back(200);
    cubes.emplace_back(200);
    cubes[1].setPosition({400, 200});
    cubes[2].setPosition({600, 200});
}

void GameScene::update(float& deltaTime) {
    for (auto& cube : cubes) {
        cube.rotate(0.01, 0, 0);
        cube.update(deltaTime);
    }
}

void GameScene::render(sf::RenderWindow& window) const {
    window.clear(sf::Color::Black);

    for (const auto& cube : cubes) {
        cube.render(window);
    }
}

void GameScene::onResize(unsigned int width, unsigned int height) {}

void GameScene::onClick(sf::Vector2f position) {}
