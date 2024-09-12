#include <iostream>
#include "World.h"
#include "../Config.h"

World::World(): renderDistance(Config::World::RENDER_DISTANCE), skyColor(Config::World::SKY_COLOR),
                chunkSize(Config::World::CHUNK_SIZE) {}

// Initialize the world by generating chunks
void World::init() {
    for (int x = -renderDistance; x < renderDistance; x++) {
        for (int z = -renderDistance; z < renderDistance; z++) {
            generateChunkAt(x * chunkSize, z * chunkSize);  // Generate chunk at world coordinates
        }
    }
}

void World::update(float deltaTime) {
    // Currently empty, could update blocks in the future
}

void World::render() const {
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Iterate through all the chunks and render them
    for (const auto& [chunkPos, chunk] : chunks) {
        chunk.render();  // Render each chunk
    }

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);
}

// Check if a player AABB collides with any blocks in the world
bool World::checkCollision(const Math::AABB& playerAABB) const {
    for (const auto& [chunkPos, chunk] : chunks) {
        if (chunk.checkCollision(playerAABB)) {
            return true;
        }
    }

    return false;
}

// Set a block at a specific world position
void World::setBlockAt(const sf::Vector3i& position, BlockType type) {
    Chunk* chunk = getChunkAt(position);  // Get the chunk for the specified position
    if (chunk) {
        // Calculate local chunk coordinates from the global position
        int localX = position.x % chunkSize;
        int localZ = position.z % chunkSize;

        // Handle negative modulus by adding chunkSize
        if (localX < 0) localX += chunkSize;
        if (localZ < 0) localZ += chunkSize;

        // Set the block in the chunk
        chunk->setBlockAt({localX, position.y, localZ}, type);
    }
}

// Get the block at a specific world position
const Block* World::getBlockAt(const sf::Vector3i& position) const {
    int chunkX = (position.x < 0) ? (position.x - chunkSize + 1) / chunkSize : position.x / chunkSize;
    int chunkZ = (position.z < 0) ? (position.z - chunkSize + 1) / chunkSize : position.z / chunkSize;

    sf::Vector2i chunkPos(chunkX, chunkZ);  // Calculate chunk coordinates
    auto it = chunks.find(chunkPos);
    if (it != chunks.end()) {
        // Calculate local chunk coordinates
        int localX = position.x % chunkSize;
        int localZ = position.z % chunkSize;

        // Handle negative modulus by adding chunkSize
        if (localX < 0) localX += chunkSize;
        if (localZ < 0) localZ += chunkSize;

        // Return the block from the chunk
        return it->second.getBlockAt({localX, position.y, localZ});
    }
    return nullptr;
}

// Remove a block at a specific world position
void World::removeBlockAt(const sf::Vector3i& position) {
    Chunk* chunk = getChunkAt(position);  // Get the chunk for the specified position
    if (chunk) {
        // Calculate local chunk coordinates from the global position
        int localX = position.x % chunkSize;
        int localZ = position.z % chunkSize;

        // Handle negative modulus by adding chunkSize
        if (localX < 0) localX += chunkSize;
        if (localZ < 0) localZ += chunkSize;

        // Remove the block from the chunk
        chunk->removeBlockAt({localX, position.y, localZ});
    }
}

// Helper function to get the chunk containing the specified world position
Chunk* World::getChunkAt(const sf::Vector3i& position) {
    int chunkX = (position.x < 0) ? (position.x - chunkSize + 1) / chunkSize : position.x / chunkSize;
    int chunkZ = (position.z < 0) ? (position.z - chunkSize + 1) / chunkSize : position.z / chunkSize;

    sf::Vector2i chunkPos(chunkX, chunkZ);  // Calculate chunk coordinates
    auto it = chunks.find(chunkPos);
    if (it != chunks.end()) {
        return &it->second;
    }
    return nullptr;  // Return nullptr if the chunk doesn't exist
}

// Generate a chunk at the specified world coordinates (x, z)
void World::generateChunkAt(int x, int z) {
    // Calculate the chunk's world coordinates based on its grid position
    sf::Vector2i chunkPos(x / chunkSize, z / chunkSize);  // Calculate chunk grid coordinates

    Chunk chunk;
    chunk.generate(chunkPos.x * chunkSize, chunkPos.y * chunkSize);  // Pass absolute world coordinates
    chunks[chunkPos] = std::move(chunk);  // Move the generated chunk into the map
}