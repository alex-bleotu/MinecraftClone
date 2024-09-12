#ifndef MINECRAFTCLONE_WORLD_H
#define MINECRAFTCLONE_WORLD_H


#include <vector>
#include <unordered_map>
#include "Block.h"
#include "../Utils/Math.h"
#include "../Player/Player.h"

class Player;

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

    // Get the block at a specific position in the world
    [[nodiscard]] const Block* getBlockAt(const sf::Vector3i& position) const;

    // Check if a player AABB collides with any blocks in the world
    bool checkCollision(const Math::AABB& playerAABB) const;

    // Check if a player AABB collides with a specific block AABB
    bool checkCollision(const Math::AABB& playerAABB, const Math::AABB& blockAABB) const;

    // Set a block at a specific position
    void setBlockAt(const sf::Vector3i& position, BlockType type);

    // Remove a block at a specific position
    void removeBlockAt(const sf::Vector3i& position);

    // Raycast from the player's position to find the first block hit
    std::tuple<sf::Vector3i, sf::Vector3f, sf::Vector3f> raycast(const Player& player, float maxDistance) const;

    // Break a block at the player's position
    void breakBlock(const Player& player);

    //  Place a block at the player's position
    void placeBlock(const Player& player, BlockType blockType);

    // Get the normal of the block face the player is looking at
    sf::Vector3f getBlockFaceNormal(const sf::Vector3f& hitPoint, const sf::Vector3i& blockPos) const;

private:
    std::unordered_map<sf::Vector3i, Block> blocks;  // List of blocks in the world

    const sf::Vector3f skyColor = {0.431f, 0.694f, 1.0f};
};


#endif
