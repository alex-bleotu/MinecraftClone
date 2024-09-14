#include <iostream>
#include <filesystem>
#include "Texture.h"
#include "../Core/Block.h"

sf::Texture Texture::atlas;

void Texture::loadTextures() {
    std::string path = "assets/textures/";

    // Load the atlas texture
    Texture::atlas.loadFromFile(path + "blocks.png");
}

std::pair<std::vector<sf::IntRect>, std::vector<int>> Texture::initTextures(BlockType type) {
    if (type == BlockType::GRASS) {
        return {
                // Texture coordinates
                {
                        getTextureCoords("grass_side"),    // front
                        getTextureCoords("grass_side"),    // back
                        getTextureCoords("dirt"),          // bottom
                        getTextureCoords("grass"),         // top
                        getTextureCoords("grass_side"),    // left
                        getTextureCoords("grass_side")     // right
                },
                // Rotation angles for each face (in degrees)
                {
                        0,    // front
                        0,    // back
                        0,    // bottom
                        0,    // top
                        90,   // left
                        90,   // right
                }
        };
    } else if (type == BlockType::DIRT) {
        return {
                {
                    getTextureCoords("dirt"),
                    getTextureCoords("dirt"),
                    getTextureCoords("dirt"),
                    getTextureCoords("dirt"),
                    getTextureCoords("dirt"),
                    getTextureCoords("dirt")
                },
                {
                    180, 180, 0, 0, -90, -90
                }
        };
    } else if (type == BlockType::STONE) {
        return {
                {
                    getTextureCoords("stone"),
                    getTextureCoords("stone"),
                    getTextureCoords("stone"),
                    getTextureCoords("stone"),
                    getTextureCoords("stone"),
                    getTextureCoords("stone")
                },
                {
                    180, 180, 0, 0, -90, -90
                }
        };
    } else if (type == BlockType::WATER) {
        return {
                {
                    getTextureCoords("water"),
                    getTextureCoords("water"),
                    getTextureCoords("water"),
                    getTextureCoords("water"),
                    getTextureCoords("water"),
                    getTextureCoords("water")
                },
                {
                    180, 180, 0, 0, -90, -90
                }
        };
    } else if (type == BlockType::PLANKS) {
        return {
                {
                    getTextureCoords("planks"),
                    getTextureCoords("planks"),
                    getTextureCoords("planks"),
                    getTextureCoords("planks"),
                    getTextureCoords("planks"),
                    getTextureCoords("planks")
                },
                {
                    180, 180, 0, 0, -90, -90
                }
        };
    } else {
        return {
                {
                    getTextureCoords("none"),
                    getTextureCoords("none"),
                    getTextureCoords("none"),
                    getTextureCoords("none"),
                    getTextureCoords("none"),
                    getTextureCoords("none")
                },
                {
                    180, 180, 0, 0, -90, -90
                }
        };
    }
}

// Function to return the texture coordinates from the atlas
sf::IntRect Texture::getTextureCoords(const std::string& name) {
    // Atlas is assumed to be 256x256, each tile is 16x16
    const int tileSize = 16;

    if (name == "dirt") return {0, 0, tileSize, tileSize};
    else if (name == "grass_side") return {16, 0, tileSize, tileSize};
    else if (name == "grass") return {32, 0, tileSize, tileSize};
    else if (name == "stone") return {48, 0, tileSize, tileSize};
    else if (name == "planks") return {0, 16, tileSize, tileSize};
    else if (name == "water") return {16, 16, tileSize, tileSize};
    else return {48, 48, tileSize, tileSize};  // Default to 'none'
}