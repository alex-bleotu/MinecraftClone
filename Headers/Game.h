#ifndef MINECRAFTCLONE_GAME_H
#define MINECRAFTCLONE_GAME_H

#include <SFML/Graphics.hpp>
#include "Scene.h"

class Game {
private:
    sf::RenderWindow window;
    Scene* currentScene;

public:
    Game();
    ~Game() = default;

    // The loop that runs the game
    void run();
};


#endif
