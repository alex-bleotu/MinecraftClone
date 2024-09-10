#ifndef MINECRAFTCLONE_GAME_H
#define MINECRAFTCLONE_GAME_H

#include <SFML/Graphics.hpp>
#include "src/UI/Scene.h"
#include <SFML/OpenGL.hpp>

class Game {
private:
    sf::RenderWindow window;
    Scene* currentScene;

public:
    Game();
    ~Game() = default;

    bool running;

    // The loop that runs the game
    void run();
};


#endif
