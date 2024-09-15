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
                        90,   // back
                        0,    // bottom
                        0,    // top
                        90,   // left
                        0,    // right
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
                    180, -90, 0, 0, -90, 180
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
                    180, -90, 0, 0, -90, 180
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
                    180, -90, 0, 0, -90, 180
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
                    180, -90, 0, 0, -90, 180
                }
        };
    } else if (type == BlockType::COBBLESTONE) {
        return {
                {
                    getTextureCoords("cobblestone"),
                    getTextureCoords("cobblestone"),
                    getTextureCoords("cobblestone"),
                    getTextureCoords("cobblestone"),
                    getTextureCoords("cobblestone"),
                    getTextureCoords("cobblestone")
                },
                {
                    180, -90, 0, 0, -90, 180
                }
        };
    } else if (type == BlockType::LEAVES) {
        return {
                {
                    getTextureCoords("leaves"),
                    getTextureCoords("leaves"),
                    getTextureCoords("leaves"),
                    getTextureCoords("leaves"),
                    getTextureCoords("leaves"),
                    getTextureCoords("leaves")
                },
                {
                    180, -90, 0, 0, -90, 180
                }
        };
    } else if (type == BlockType::LOG) {
        return {
                {
                    getTextureCoords("log"),
                    getTextureCoords("log"),
                    getTextureCoords("log_top"),
                    getTextureCoords("log_top"),
                    getTextureCoords("log"),
                    getTextureCoords("log")
                },
                {
                    0, 90, 0, 0, 90, 0
                }
        };
    } else if (type == BlockType::CRAFTING_TABLE) {
        return {
                {
                    getTextureCoords("crafting_table_front"),
                    getTextureCoords("crafting_table_front"),
                    getTextureCoords("planks"),
                    getTextureCoords("crafting_table_top"),
                    getTextureCoords("crafting_table_side"),
                    getTextureCoords("crafting_table_side")
                },
                {
                    0, 90, 0, 0, 90, 0
                }
        };
    } else if (type == BlockType::FURNACE) {
        return {
                {
                    getTextureCoords("furnace_front"),
                    getTextureCoords("furnace_side"),
                    getTextureCoords("furnace_top"),
                    getTextureCoords("furnace_top"),
                    getTextureCoords("furnace_side"),
                    getTextureCoords("furnace_side")
                },
                {
                    0, 90, 0, 0, 90, 0
                }
        };
    } else if (type == BlockType::IRON_ORE) {
        return {
                {
                    getTextureCoords("iron_ore"),
                    getTextureCoords("iron_ore"),
                    getTextureCoords("iron_ore"),
                    getTextureCoords("iron_ore"),
                    getTextureCoords("iron_ore"),
                    getTextureCoords("iron_ore")
                },
                {
                    180, -90, 0, 0, -90, 180
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
                    180, -90, 0, 0, -90, 180
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
    else if (name == "cobblestone") return { 64, 0, tileSize, tileSize };
    else if (name == "planks") return {80, 0, tileSize, tileSize};
    else if (name == "log") return {96, 0, tileSize, tileSize};
    else if (name == "log_top") return {112, 0, tileSize, tileSize};
    else if (name == "leaves") return {0, 16, tileSize, tileSize};
    else if (name == "crafting_table_front") return {16, 16, tileSize, tileSize};
    else if (name == "crafting_table_side") return {32, 16, tileSize, tileSize};
    else if (name == "crafting_table_top") return {48, 16, tileSize, tileSize};
    else if (name == "furnace_front") return { 64, 16, tileSize, tileSize };
    else if (name == "furnace_side") return {80, 16, tileSize, tileSize};
    else if (name == "furnace_top") return {96, 16, tileSize, tileSize};
    else if (name == "furnace_front_lit") return {112, 16, tileSize, tileSize};
    else if (name == "water") return {0, 32, tileSize, tileSize};
    else if (name == "iron_ore") return {16, 32, tileSize, tileSize};

    else return {112, 112, tileSize, tileSize};  // Default to 'none'
}