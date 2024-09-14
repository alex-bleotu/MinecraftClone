#include <iostream>
#include "Chunk.h"
#include "../Utils/PerlinNoise.h"
#include "../Config.h"

// Constructor for the chunk
Chunk::Chunk(): chunkSize(Config::World::CHUNK_SIZE) {}

// Generate the chunk using Perlin noise for terrain generation
void Chunk::generate(int xOffset, int zOffset, const PerlinNoise& noiseGenerator) {
//    // Parameters for multi-octave noise
//    const float frequency = 0.05f;
//    const int octaves = 4;
//    const float persistence = 0.5f;
//
//    // Iterate through the chunk's x and z coordinates
//    for (int x = 0; x < chunkSize; x++) {
//        for (int z = 0; z < chunkSize; z++) {
//            // Calculate the world coordinates for this block
//            int worldX = xOffset + x;
//            int worldZ = zOffset + z;
//
//            // Sample from the larger Perlin noise map
//            float noiseValue = 0.0f;
//            float amplitude = 1.0f;
//            float maxValue = 0.0f;
//
//            // Loop through each octave
//            for (int octave = 0; octave < octaves; octave++) {
//                float freq = frequency * std::pow(2.0f, octave);
//
//                // Use the same worldX and worldZ for all chunks, ensuring the seed-based noise
//                noiseValue += noiseGenerator.noise(worldX * freq, worldZ * freq, 0.5f) * amplitude;
//                maxValue += amplitude;
//                amplitude *= persistence;
//            }
//
//            // Normalize the noise value
//            noiseValue /= maxValue;
//
//            // Adjust the height scale
//            float height = noiseValue * 20.0f;  // Adjust this value for terrain height
//            int groundHeight = static_cast<int>(height);
//
//            // Fill the chunk with blocks up to the calculated height
//            for (int y = 0; y < groundHeight; y++) {
//                sf::Vector3i blockPos(worldX, y, worldZ);
//
//                if (y == groundHeight - 1) {
//                    blocks[blockPos] = Block(BlockType::GRASS, blockPos);  // Topmost block is grass
//                } else if (y >= groundHeight - 4) {
//                    blocks[blockPos] = Block(BlockType::DIRT, blockPos);  // Next few layers are dirt
//                } else {
//                    blocks[blockPos] = Block(BlockType::STONE, blockPos);  // Below that is stone
//                }
//            }
//        }
//    }
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

// Check if a player AABB collides with any blocks in the chunk
bool Chunk::checkCollision(const Math::AABB& playerAABB) const {
    // Iterate through all the blocks in the chunk
    for (const auto& [position, block] : blocks) {
        // Check if the player AABB collides with the block's AABB
        if (block.getAABB().intersects(playerAABB)) {
            return true;  // Collision detected
        }
    }
    return false;  // No collision detected
}