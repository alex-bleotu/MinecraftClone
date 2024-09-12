#ifndef MINECRAFTCLONE_WORLD_H
#define MINECRAFTCLONE_WORLD_H


#include <vector>
#include <unordered_map>
#include "Block.h"
#include "../Utils/Math.h"
#include "../Player/Player.h"
#include "Chunk.h"

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
    void render() const;

    // Check if a player AABB collides with any blocks in the world
    bool checkCollision(const Math::AABB& playerAABB) const;

    // Get the block at a specific position in the world
    [[nodiscard]] const Block* getBlockAt(const sf::Vector3i& position) const;

    // Set a block at a specific position
    void setBlockAt(const sf::Vector3i& position, BlockType type);

    // Remove a block at a specific position
    void removeBlockAt(const sf::Vector3i& position);

private:
    // Helper function to get the chunk containing the specified position
    Chunk* getChunkAt(const sf::Vector3i& position);

    // Generate a chunk at a specified world position
    void generateChunkAt(int x, int z);

    // Store chunks in the world (keyed by chunk position)
    std::unordered_map<sf::Vector2i, Chunk> chunks;

    // Define the render distance (how many chunks around the player are generated and rendered)
    const int renderDistance;

    // Define the sky color
    const sf::Vector3f skyColor;

    // Define the size of each chunk
    const int chunkSize;
};


#endif
