#include "World.h"

World::World() {
    // Initialize the world (can be extended)
}

void World::init() {
    // Example: Add blocks to the world at different positions
    for (int i = -20; i < 30; i++)
        for (int j = -20; j < 30; j++)
            blocks.emplace_back(Block(BlockType::GRASS, {i, 0, j}));

    for (int i = 1; i <= 3; i++) {
        blocks.emplace_back(Block(BlockType::DIRT, {6, i, 2}));
        blocks.emplace_back(Block(BlockType::PLANKS, {5, i, 2}));
        blocks.emplace_back(Block(BlockType::PLANKS, {4, i, 2}));
        blocks.emplace_back(Block(BlockType::PLANKS, {3, i, 2}));
        blocks.emplace_back(Block(BlockType::DIRT, {2, i, 2}));
        blocks.emplace_back(Block(BlockType::DIRT, {6, i, 6}));
        blocks.emplace_back(Block(BlockType::PLANKS, {5, i, 6}));
        blocks.emplace_back(Block(BlockType::PLANKS, {3, i, 6}));
        blocks.emplace_back(Block(BlockType::DIRT, {2, i, 6}));
        blocks.emplace_back(Block(BlockType::PLANKS, {2, i, 5}));
        blocks.emplace_back(Block(BlockType::PLANKS, {2, i, 4}));
        blocks.emplace_back(Block(BlockType::PLANKS, {2, i, 3}));
        blocks.emplace_back(Block(BlockType::PLANKS, {6, i, 5}));
        blocks.emplace_back(Block(BlockType::PLANKS, {6, i, 4}));
        blocks.emplace_back(Block(BlockType::PLANKS, {6, i, 3}));
    }
    blocks.emplace_back(Block(BlockType::PLANKS, {4, 3, 6}));
    blocks.emplace_back(Block(BlockType::PLANKS, {3, 3, 3}));
    blocks.emplace_back(Block(BlockType::PLANKS, {3, 3, 4}));
    blocks.emplace_back(Block(BlockType::PLANKS, {3, 3, 5}));
    blocks.emplace_back(Block(BlockType::PLANKS, {4, 3, 3}));
    blocks.emplace_back(Block(BlockType::PLANKS, {5, 3, 3}));
    blocks.emplace_back(Block(BlockType::PLANKS, {4, 3, 4}));
    blocks.emplace_back(Block(BlockType::PLANKS, {5, 3, 5}));
    blocks.emplace_back(Block(BlockType::PLANKS, {4, 3, 5}));
    blocks.emplace_back(Block(BlockType::PLANKS, {5, 3, 4}));
    blocks.emplace_back(Block(BlockType::WATER, {1, 1, 1}));
    blocks.emplace_back(Block(BlockType::WATER, {0, 1, 1}));

}

void World::update(float deltaTime) {
    // Currently empty, could update blocks in the future
}

void World::render(sf::RenderWindow& window) const {
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Render each block in the world
    for (const auto& block : blocks) {
        block.render(*this);
    }

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);
}

const Block* World::getBlockAt(const sf::Vector3i& position) const {
    // Find the block at the given position
    for (const auto& block : blocks) {
        if (block.getPosition() == position) {
            return const_cast<Block*>(&block);
        }
    }

    return nullptr;
}