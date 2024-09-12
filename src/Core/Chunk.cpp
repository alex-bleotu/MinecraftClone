#include "Chunk.h"
#include "../Utils/PerlinNoise.h"

// Constructor for the chunk
Chunk::Chunk() {}

// Generate the chunk using Perlin noise for terrain generation
void Chunk::generate(int xOffset, int zOffset) {
    PerlinNoise noise;

    // Iterate through the chunk's x and z coordinates
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int z = 0; z < CHUNK_SIZE; z++) {
            // Generate height using Perlin noise, scaled for terrain height
            float height = noise.noise((xOffset + x) * 0.1f, (zOffset + z) * 0.1f, 0.5f) * 10.0f;

            // Fill the chunk with blocks up to the calculated height
            for (int y = 0; y < static_cast<int>(height); ++y) {
                sf::Vector3i blockPos(x, y, z);
                blocks[blockPos] = Block(BlockType::STONE, blockPos);  // Stone block type, adjust as needed
            }
        }
    }
}

// Retrieve the block at a specific position within the chunk
const Block* Chunk::getBlockAt(const sf::Vector3i& position) const {
    auto it = blocks.find(position);
    if (it != blocks.end()) {
        return &it->second;
    }
    return nullptr;  // Return null if no block is found at the given position
}

// Set a block at a specific position within the chunk
void Chunk::setBlockAt(const sf::Vector3i& position, BlockType type) {
    blocks[position] = Block(type, position);  // Set the block to the desired type
}

// Remove a block at a specific position within the chunk
void Chunk::removeBlockAt(const sf::Vector3i& position) {
    blocks.erase(position);  // Remove the block from the chunk
}

// Render the chunk
void Chunk::render() const {
    // Iterate through all the blocks in the chunk and render them
    for (const auto& [position, block] : blocks) {
        // Use the block's render function
        block.render();
    }
}