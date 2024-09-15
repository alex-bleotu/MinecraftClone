#ifndef MINECRAFTCLONE_CHUNK_H
#define MINECRAFTCLONE_CHUNK_H

#include <unordered_map>
#include "Block.h"
#include "../Utils/Math.h"
#include "../Utils/PerlinNoise.h"
#include <SFML/System/Vector3.hpp>

class Chunk {
public:
    Chunk();

    // Generate the chunk using Perlin noise for terrain generation
    void generate(int xOffset, int zOffset, const PerlinNoise& noiseGenerator);

    // Get the block at a specific position within the chunk
    const Block* getBlockAt(const sf::Vector3i& position) const;

    // Set a block at a specific position within the chunk
    void setBlockAt(const sf::Vector3i& position, BlockType type);

    // Remove a block at a specific position within the chunk
    void removeBlockAt(const sf::Vector3i& position);

    // Check if a player AABB collides with any blocks in the chunk
    bool checkCollision(const Math::AABB& playerAABB) const;

    // Render the chunk
    void render() const;

    // Render the chunk
    void renderNotOpaque() const;

    // Get the chunk's AABB
    Math::AABB getAABB() const;

private:
    std::unordered_map<sf::Vector3i, Block> blocks;  // List of blocks in the chunk

    int chunkSize; // Size of the chunk

    sf::Vector2i position;  // Position of the chunk in the world
};

#endif
