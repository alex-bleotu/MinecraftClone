#ifndef MINECRAFTCLONE_WORLD_H
#define MINECRAFTCLONE_WORLD_H


#include <vector>
#include "Block.h"

class World {
public:
    // Constructor to initialize the world
    World();

    // Initialize the world with blocks
    void init();

    // Update the world (could handle block updates in the future)
    void update(float deltaTime);

    // Render all blocks in the world
    void render(sf::RenderWindow& window) const;

private:
    std::vector<Block> blocks;  // List of blocks in the world

    const sf::Color skyColor = sf::Color(120, 167, 255);
};


#endif
