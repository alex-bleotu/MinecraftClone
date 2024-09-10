#include "World.h"

World::World() {
    // Initialize the world (can be extended)
}

void World::init() {
    // Example: Add blocks to the world at different positions
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            if (i % 2 == 0 && j % 2 == 0)
                blocks.emplace_back(Block(BlockType::GRASS, {i, 0, j}));
            else
                blocks.emplace_back(Block(BlockType::DIRT, {i, 0, j}));
}

void World::update(float deltaTime) {
    // Currently empty, could update blocks in the future
}

void World::render() const {
    // Render each block in the world
    for (const auto& block : blocks) {
        block.render();
    }
}