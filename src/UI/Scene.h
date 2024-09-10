#ifndef MINECRAFTCLONE_SCENE_H
#define MINECRAFTCLONE_SCENE_H


#include <functional>
#include <Graphics/RenderWindow.hpp>
#include "UserInterface.h"
#include "../Core/Block.h"
#include "../Core/World.h"
#include "../Rendering/Camera.h"

class Scene {
private:
    std::function<void(Scene*)> sceneChanger;

protected:
    sf::RenderWindow& window;

    [[nodiscard]] const std::function<void(Scene*)>& getSceneChanger() const;

public:
    explicit Scene(std::function<void(Scene*)> sceneChanger, sf::RenderWindow& window);

    virtual void update(float& deltaTime) = 0;
    virtual void render() const = 0;

    virtual void onResize(unsigned int width, unsigned int height) = 0;
    virtual void onClick(sf::Vector2f position) = 0;
};

class MenuScene : public Scene {
    UI::Layout layout;
public:
    explicit MenuScene(std::function<void(Scene *)> sceneChanger, sf::RenderWindow& window);

    void update(float& deltaTime) override;
    void render() const override;

    void onResize(unsigned int width, unsigned int height) override;
    void onClick(sf::Vector2f position) override;
};

class GameScene : public Scene {
    World world;  // The game world that contains blocks
    Camera camera;  // The camera to move around the world
public:
    explicit GameScene(std::function<void(Scene*)> sceneChanger, sf::RenderWindow& window);

    void update(float& deltaTime) override;
    void render() const override;

    void onResize(unsigned int width, unsigned int height) override;
    void onClick(sf::Vector2f position) override;
};


#endif
