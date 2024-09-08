#ifndef MINECRAFTCLONE_SCENE_H
#define MINECRAFTCLONE_SCENE_H


#include <functional>
#include <Graphics/RenderWindow.hpp>
#include "UserInterface.h"
#include "Cube.h"

class Scene {
private:
    std::function<void(Scene*)> sceneChanger;

protected:
    [[nodiscard]] const std::function<void(Scene*)>& getSceneChanger() const;

public:
    explicit Scene(std::function<void(Scene*)> sceneChanger);

    virtual void update(float& deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) const = 0;

    virtual void onResize(unsigned int width, unsigned int height) = 0;
    virtual void onClick(sf::Vector2f position) = 0;
};

class MenuScene : public Scene {
    UI::Layout layout;
public:
    explicit MenuScene(std::function<void(Scene *)> sceneChanger);

    void update(float& deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void onResize(unsigned int width, unsigned int height) override;
    void onClick(sf::Vector2f position) override;
};

class GameScene : public Scene {
    std::vector<Cube> cubes;
public:
    explicit GameScene(std::function<void(Scene*)> sceneChanger);

    void update(float& deltaTime) override;
    void render(sf::RenderWindow& window) const override;

    void onResize(unsigned int width, unsigned int height) override;
    void onClick(sf::Vector2f position) override;
};


#endif
