#include "World.h"

World::World() {}

void World::init() {
    // Example: Add blocks to the world at different positions using unordered_map

    // Create a flat grassy area (-20 to 30 on the X and Z axes)
    for (int i = -20; i < 30; i++) {
        for (int j = -20; j < 30; j++) {
            setBlockAt({i, 0, j}, BlockType::GRASS);  // Set grass blocks at ground level
        }
    }

    // Build a small structure with dirt and planks
    for (int i = 1; i <= 3; i++) {
        setBlockAt({6, i, 2}, BlockType::DIRT);
        setBlockAt({5, i, 2}, BlockType::PLANKS);
        setBlockAt({4, i, 2}, BlockType::PLANKS);
        setBlockAt({3, i, 2}, BlockType::PLANKS);
        setBlockAt({2, i, 2}, BlockType::DIRT);
        setBlockAt({6, i, 6}, BlockType::DIRT);
        setBlockAt({5, i, 6}, BlockType::PLANKS);
        setBlockAt({3, i, 6}, BlockType::PLANKS);
        setBlockAt({2, i, 6}, BlockType::DIRT);
        setBlockAt({2, i, 5}, BlockType::PLANKS);
        setBlockAt({2, i, 4}, BlockType::PLANKS);
        setBlockAt({2, i, 3}, BlockType::PLANKS);
        setBlockAt({6, i, 5}, BlockType::PLANKS);
        setBlockAt({6, i, 4}, BlockType::PLANKS);
        setBlockAt({6, i, 3}, BlockType::PLANKS);
    }

    // Set individual blocks for the rest of the structure
    setBlockAt({4, 3, 6}, BlockType::PLANKS);
    setBlockAt({3, 3, 3}, BlockType::PLANKS);
    setBlockAt({3, 3, 4}, BlockType::PLANKS);
    setBlockAt({3, 3, 5}, BlockType::PLANKS);
    setBlockAt({4, 3, 3}, BlockType::PLANKS);
    setBlockAt({5, 3, 3}, BlockType::PLANKS);
    setBlockAt({4, 3, 4}, BlockType::PLANKS);
    setBlockAt({5, 3, 5}, BlockType::PLANKS);
    setBlockAt({4, 3, 5}, BlockType::PLANKS);
    setBlockAt({5, 3, 4}, BlockType::PLANKS);

    // Add some water blocks
    setBlockAt({1, 1, 1}, BlockType::WATER);
    setBlockAt({0, 1, 1}, BlockType::WATER);
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
    for (const auto& [position, block] : blocks) {
        block.render(*this);
    }

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);
}

const Block* World::getBlockAt(const sf::Vector3i& position) const {
    // Find the block at the given position
    for (const auto& [position, block] : blocks) {
        if (block.getPosition() == position) {
            return const_cast<Block*>(&block);
        }
    }

    return nullptr;
}

bool World::checkCollision(const Math::AABB& playerAABB) const {
    float epsilon = 0.001f;  // Small buffer to avoid floating-point precision issues

    for (const auto& [position, block] : blocks) {
        Math::AABB blockAABB = block.getBoundingBox();

        // Check collision between playerAABB and blockAABB, with a small epsilon buffer
        if ((playerAABB.max.x > blockAABB.min.x - epsilon && playerAABB.min.x < blockAABB.max.x + epsilon) &&
            (playerAABB.max.y > blockAABB.min.y - epsilon && playerAABB.min.y < blockAABB.max.y + epsilon) &&
            (playerAABB.max.z > blockAABB.min.z - epsilon && playerAABB.min.z < blockAABB.max.z + epsilon)) {
            return true;
        }
    }

    return false;
}

void World::setBlockAt(const sf::Vector3i& position, BlockType type) {
    // Check if block already exists at this position
    if (blocks.find(position) != blocks.end()) {
        // If it exists, update its type
        blocks[position].setType(type);
    } else {
        // Otherwise, create a new block at the position
        Block newBlock(type, position);
        blocks[position] = newBlock;
    }
}

const Block* World::getBlockAt(const sf::Vector3i& position) const {
    auto it = blocks.find(position);
    if (it != blocks.end()) {
        return &it->second;  // Return a pointer to the block
    }
    return nullptr;  // Return nullptr if no block exists at this position
}

void World::removeBlockAt(const sf::Vector3i& position) {
    blocks.erase(position);  // Erase the block at the given position
}