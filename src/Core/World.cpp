#include <iostream>
#include "World.h"
#include "../Config.h"

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

sf::Vector3i World::raycast(const Player& player, float maxDistance) const {
    // Get the player's current position and look direction
    sf::Vector3f rayOrigin = player.getPosition();

    rayOrigin.y += player.getIsCrouching() ? Config::Player::CROUCH_HEIGHT : Config::Player::NORMAL_HEIGHT;
    rayOrigin.y -= 0.1f;

    sf::Vector3f rayDirection = player.getLookDirection();  // Assume this method exists

    // Normalize the direction vector
    rayDirection /= std::sqrt(rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z);

    // Ray step increment
    float step = 0.1f;  // Small steps for precision
    sf::Vector3f ray = rayOrigin;

    // Step along the ray until a block is hit or max distance is exceeded
    for (float distance = 0.0f; distance < maxDistance; distance += step) {
        ray += rayDirection * step;

        sf::Vector3i blockPos = sf::Vector3i(std::floor(ray.x), std::floor(ray.y), std::floor(ray.z));

        // Check if a block exists at this position and is visible
        if (const Block* block = getBlockAt(blockPos)) {
            if (block->isVisible()) {

                return blockPos;  // Return the position of the block hit by the ray
            }
        }
    }

    return sf::Vector3i(-1000, -1000, -1000);  // Return invalid position if no block was found
}

void World::breakBlock(const Player& player) {
    // Raycast to find the block the player is looking at
    sf::Vector3i blockPos = raycast(player, 5.0f);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Remove the block from the map
        removeBlockAt(blockPos);
    }
}

void World::placeBlock(const Player& player, BlockType blockType) {
    // Raycast to find the block the player is looking at
    sf::Vector3i blockPos = raycast(player, 5.0f);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Find the adjacent position to place the block
        sf::Vector3f normal = getBlockFaceNormal(player, blockPos);  // Assume this method exists
        sf::Vector3i newBlockPos = blockPos + sf::Vector3i(normal.x, normal.y, normal.z);

        // Place the block in the world
        setBlockAt(newBlockPos, blockType);
    }
}

sf::Vector3f World::getBlockFaceNormal(const Player& player, const sf::Vector3i& blockPos) const {
    sf::Vector3f playerPos = player.getPosition();
    sf::Vector3f blockCenter = sf::Vector3f(blockPos.x + 0.5f, blockPos.y + 0.5f, blockPos.z + 0.5f);

    // Calculate the direction from the block center to the player
    sf::Vector3f direction = playerPos - blockCenter;

    // Find the largest component of the direction vector
    if (std::abs(direction.x) > std::abs(direction.y) && std::abs(direction.x) > std::abs(direction.z)) {
        return {(direction.x > 0) ? 1.0f : -1.0f, 0.0f, 0.0f};  // Left or right face
    } else if (std::abs(direction.y) > std::abs(direction.x) && std::abs(direction.y) > std::abs(direction.z)) {
        return {0.0f, (direction.y > 0) ? 1.0f : -1.0f, 0.0f};  // Top or bottom face
    } else {
        return {0.0f, 0.0f, (direction.z > 0) ? 1.0f : -1.0f};  // Front or back face
    }
}