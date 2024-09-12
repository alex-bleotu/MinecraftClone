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
        Math::AABB blockAABB = block.getAABB();

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

std::tuple<sf::Vector3i, sf::Vector3f, sf::Vector3f> World::raycast(const Player& player, float maxDistance) const {
    // Get the player's current position and look direction
    sf::Vector3f rayOrigin = player.getPosition();
    rayOrigin.y += player.getIsCrouching() ? Config::Player::CROUCH_HEIGHT : Config::Player::NORMAL_HEIGHT;
    rayOrigin.y -= 0.1f;  // Adjust the ray origin to start at eye level

    sf::Vector3f rayDirection = player.getLookDirection();
    rayDirection /= std::sqrt(rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z);  // Normalize the direction vector

    // Initialize starting position (in block coordinates)
    sf::Vector3i blockPos = sf::Vector3i(std::floor(rayOrigin.x), std::floor(rayOrigin.y), std::floor(rayOrigin.z));

    // Variables for stepping through the blocks
    sf::Vector3f tMax;  // Distance to the next block boundary
    sf::Vector3f tDelta;  // How far to step in each direction
    sf::Vector3f step;

    // Calculate the step direction and tMax/tDelta for each axis (X, Y, Z)
    if (rayDirection.x > 0) {
        step.x = 1;
        tMax.x = ((blockPos.x + 1) - rayOrigin.x) / rayDirection.x;
        tDelta.x = 1.0f / std::abs(rayDirection.x);
    } else {
        step.x = -1;
        tMax.x = (rayOrigin.x - blockPos.x) / -rayDirection.x;
        tDelta.x = 1.0f / std::abs(rayDirection.x);
    }

    if (rayDirection.y > 0) {
        step.y = 1;
        tMax.y = ((blockPos.y + 1) - rayOrigin.y) / rayDirection.y;
        tDelta.y = 1.0f / std::abs(rayDirection.y);
    } else {
        step.y = -1;
        tMax.y = (rayOrigin.y - blockPos.y) / -rayDirection.y;
        tDelta.y = 1.0f / std::abs(rayDirection.y);
    }

    if (rayDirection.z > 0) {
        step.z = 1;
        tMax.z = ((blockPos.z + 1) - rayOrigin.z) / rayDirection.z;
        tDelta.z = 1.0f / std::abs(rayDirection.z);
    } else {
        step.z = -1;
        tMax.z = (rayOrigin.z - blockPos.z) / -rayDirection.z;
        tDelta.z = 1.0f / std::abs(rayDirection.z);
    }

    // Traverse the grid until a block is hit or max distance is exceeded
    for (float distance = 0.0f; distance < maxDistance; ) {
        // Move to the next block along the closest axis
        sf::Vector3f hitNormal(0.0f, 0.0f, 0.0f);
        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            blockPos.x += step.x;
            distance = tMax.x;
            tMax.x += tDelta.x;
            hitNormal = { -step.x, 0.0f, 0.0f };  // X-axis face was hit
        } else if (tMax.y < tMax.z) {
            blockPos.y += step.y;
            distance = tMax.y;
            tMax.y += tDelta.y;
            hitNormal = { 0.0f, -step.y, 0.0f };  // Y-axis face was hit
        } else {
            blockPos.z += step.z;
            distance = tMax.z;
            tMax.z += tDelta.z;
            hitNormal = { 0.0f, 0.0f, -step.z };  // Z-axis face was hit
        }

        // Check if the block at this position is solid (visible)
        if (const Block* block = getBlockAt(blockPos)) {
            if (block->isVisible()) {
                sf::Vector3f hitPoint = rayOrigin + rayDirection * distance;
                return { blockPos, hitPoint, hitNormal };  // Return block position, hit point, and hit face normal
            }
        }

        // If distance exceeds maxDistance, break out
        if (distance > maxDistance) {
            break;
        }
    }

    // No block hit within the max distance
    return { sf::Vector3i(-1000, -1000, -1000), sf::Vector3f(-1000.0f, -1000.0f, -1000.0f), sf::Vector3f(0.0f, 0.0f, 0.0f) };
}

void World::breakBlock(const Player& player) {
    // Raycast to find the block the player is looking at
    auto [blockPos, hitPoint, hitNormal] = raycast(player, 5.0f);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Remove the block at the position returned by the raycast
        removeBlockAt(blockPos);
    }
}

void World::placeBlock(const Player& player, BlockType blockType) {
    // Raycast to find the block the player is looking at
    auto [blockPos, hitPoint, hitNormal] = raycast(player, 5.0f);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Find the adjacent position to place the block based on hitNormal
        sf::Vector3i newBlockPos = blockPos + sf::Vector3i(hitNormal.x, hitNormal.y, hitNormal.z);

        // Get the player's bounding box (AABB)
        Math::AABB playerAABB = player.getAABB();

        // Create the new block's AABB (assuming block size is 1x1x1)
        Math::AABB blockAABB = {
                { float(newBlockPos.x), float(newBlockPos.y), float(newBlockPos.z) },
                { float(newBlockPos.x + 1), float(newBlockPos.y + 1), float(newBlockPos.z + 1) }
        };

        // Check if the player's AABB collides with the new block's AABB
        if (!checkCollision(blockAABB, playerAABB)) {
            // If no collision, place the block
            setBlockAt(newBlockPos, blockType);
        }
    }
}

sf::Vector3f World::getBlockFaceNormal(const sf::Vector3f& hitPoint, const sf::Vector3i& blockPos) const {
    // Calculate the hit point relative to the block's position
    sf::Vector3f relativeHitPoint = hitPoint - sf::Vector3f(blockPos.x, blockPos.y, blockPos.z);

    // Add a small bias to avoid issues with floating point precision
    float bias = 0.01f;

    // Determine which face the hit point is closest to
    if (std::abs(relativeHitPoint.x - 0.5f) + bias > std::abs(relativeHitPoint.y - 0.5f) &&
        std::abs(relativeHitPoint.x - 0.5f) + bias > std::abs(relativeHitPoint.z - 0.5f)) {
        // Closest to X-axis face (left or right)
        return { (relativeHitPoint.x > 0.5f) ? 1.0f : -1.0f, 0.0f, 0.0f };
    }
    else if (std::abs(relativeHitPoint.y - 0.5f) + bias > std::abs(relativeHitPoint.x - 0.5f) &&
             std::abs(relativeHitPoint.y - 0.5f) + bias > std::abs(relativeHitPoint.z - 0.5f)) {
        // Closest to Y-axis face (top or bottom)
        return { 0.0f, (relativeHitPoint.y > 0.5f) ? 1.0f : -1.0f, 0.0f };
    }
    else {
        // Closest to Z-axis face (front or back)
        return { 0.0f, 0.0f, (relativeHitPoint.z > 0.5f) ? 1.0f : -1.0f };
    }
}

bool World::checkCollision(const Math::AABB& blockAABB, const Math::AABB& playerAABB) const {
    float epsilon = 0.001f;  // Small buffer to avoid floating-point precision issues

    // Check collision between playerAABB and blockAABB, with a small epsilon buffer
    if ((playerAABB.max.x > blockAABB.min.x - epsilon && playerAABB.min.x < blockAABB.max.x + epsilon) &&
        (playerAABB.max.y > blockAABB.min.y - epsilon && playerAABB.min.y < blockAABB.max.y + epsilon) &&
        (playerAABB.max.z > blockAABB.min.z - epsilon && playerAABB.min.z < blockAABB.max.z + epsilon)) {
        return true;
    }

    return false;
}
