#include <iostream>
#include <random>
#include "World.h"
#include "../Config.h"
#include "../Utils/Texture.h"

World::World(): renderDistance(Config::World::RENDER_DISTANCE), skyColor(Config::World::SKY_COLOR),
                chunkSize(Config::World::CHUNK_SIZE), seed(std::random_device{}()), noiseGenerator(seed) {}

// Initialize the world by generating chunks
void World::init() {
    for (int x = -Config::World::CHUNKS_GENERATION; x < Config::World::CHUNKS_GENERATION; x++) {
        for (int z = -Config::World::CHUNKS_GENERATION; z < Config::World::CHUNKS_GENERATION; z++) {
            generateChunkAt(x * chunkSize, z * chunkSize);  // Generate chunk at world coordinates
        }
    }

    setBlockAt({-12, 20, 0}, BlockType::DIRT);
    setBlockAt({-10, 20, 0}, BlockType::GRASS);
    setBlockAt({-8, 20, 0}, BlockType::STONE);
    setBlockAt({-6, 20, 0}, BlockType::WATER);
    setBlockAt({-4, 20, 0}, BlockType::PLANKS);
    setBlockAt({-2, 20, 0}, BlockType::LOG);
    setBlockAt({0, 20, 0}, BlockType::COBBLESTONE);
    setBlockAt({2, 20, 0}, BlockType::LEAVES);
    setBlockAt({4, 20, 0}, BlockType::CRAFTING_TABLE);
    setBlockAt({6, 20, 0}, BlockType::FURNACE);
    setBlockAt({8, 20, 0}, BlockType::IRON_ORE);
}

void World::update(float deltaTime) {
    // Currently empty, could update blocks in the future
}

// Render the world using the player's position
void World::render(const sf::Vector3f& playerPosition) const {
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);

    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable depth testing and texture
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Enable face culling (cull back faces)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);  // Cull the back faces
    glFrontFace(GL_CW);  // Ensure counter-clockwise (CCW) is the front face

    // Bind the texture atlas
    glBindTexture(GL_TEXTURE_2D, Texture::atlas.getNativeHandle());

    // Calculate the chunk coordinates of the player
    int playerChunkX = static_cast<int>(playerPosition.x) / chunkSize;
    int playerChunkZ = static_cast<int>(playerPosition.z) / chunkSize;

    // Iterate through chunks within render distance and render opaque blocks
    for (int chunkX = playerChunkX - renderDistance; chunkX <= playerChunkX + renderDistance; chunkX++) {
        for (int chunkZ = playerChunkZ - renderDistance; chunkZ <= playerChunkZ + renderDistance; chunkZ++) {
            sf::Vector2i chunkPos(chunkX, chunkZ);

            // Check if the chunk exists in the world
            auto it = chunks.find(chunkPos);
            if (it != chunks.end()) {
                it->second.render();  // Render opaque blocks
            }
        }
    }

    // Render non-opaque blocks without culling
    glDisable(GL_CULL_FACE);  // Disable culling for transparent/non-opaque blocks

    // Iterate through chunks within render distance and render non-opaque blocks
    for (int chunkX = playerChunkX - renderDistance; chunkX <= playerChunkX + renderDistance; chunkX++) {
        for (int chunkZ = playerChunkZ - renderDistance; chunkZ <= playerChunkZ + renderDistance; chunkZ++) {
            sf::Vector2i chunkPos(chunkX, chunkZ);

            // Check if the chunk exists in the world
            auto it = chunks.find(chunkPos);
            if (it != chunks.end()) {
                it->second.renderNotOpaque();  // Render non-opaque blocks
            }
        }
    }

    // Disable depth testing and texture
    glDisable(GL_TEXTURE_2D);
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
        // Set the block in the chunk
        chunk->setBlockAt({position.x, position.y, position.z}, type);
    }
}

// Get the block at a specific world position
const Block* World::getBlockAt(const sf::Vector3i& position) const {
    int chunkX = (position.x < 0) ? (position.x - chunkSize + 1) / chunkSize : position.x / chunkSize;
    int chunkZ = (position.z < 0) ? (position.z - chunkSize + 1) / chunkSize : position.z / chunkSize;

    sf::Vector2i chunkPos(chunkX, chunkZ);  // Calculate chunk coordinates
    auto it = chunks.find(chunkPos);

    if (it != chunks.end()) {
        // Return the block from the chunk
        return it->second.getBlockAt({position.x, position.y, position.z});
    }
    return nullptr;
}

// Remove a block at a specific world position
void World::removeBlockAt(const sf::Vector3i& position) {
    Chunk* chunk = getChunkAt(position);  // Get the chunk for the specified position
    if (chunk) {
        // Remove the block from the chunk
        chunk->removeBlockAt({position.x, position.y, position.z});
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
    sf::Vector2i chunkPos(x / chunkSize, z / chunkSize);  // Calculate chunk grid coordinates

    Chunk chunk;
    chunk.generate(x, z, noiseGenerator);  // Use the global noise generator for consistent terrain
    chunks[chunkPos] = std::move(chunk);  // Move the generated chunk into the map
}