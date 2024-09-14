#include <iostream>
#include <filesystem>
#include "Texture.h"
#include <nlohmann/json.hpp>
#include <fstream>

// Define the static variables
sf::Texture Texture::grass;
sf::Texture Texture::grassSide;
sf::Texture Texture::dirt;
sf::Texture Texture::stone;
sf::Texture Texture::water;
sf::Texture Texture::planks;
sf::Texture Texture::none;

void Texture::loadTextures() {
    std::string path = "assets/textures/";

    Texture::grass.loadFromFile(path + "grass.png");
    Texture::grassSide.loadFromFile(path + "grass_side.png");
    Texture::dirt.loadFromFile(path + "dirt.png");
    Texture::stone.loadFromFile(path + "stone.png");
    Texture::water.loadFromFile(path + "water.png");
    Texture::planks.loadFromFile(path + "planks.png");
    Texture::none.loadFromFile(path + "none.png");
}

std::vector<sf::Texture> Texture::readTextures(const std::string& name) {
    std::string path = "assets/states/";

    std::ifstream file(path + name + ".json");
    nlohmann::json jsonData;

    file >> jsonData;

    // Default texture order: front, back, bottom, top, left, right
    std::vector<sf::Texture> blockTextures(6, Texture::none);

    if (jsonData.contains("variants")) {
        auto variants = jsonData["variants"];

        if (variants.contains("front") && !variants["front"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["front"]["model"].get<std::string>());
            blockTextures[0] = texture; // front
        }

        if (variants.contains("back") && !variants["back"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["back"]["model"].get<std::string>());
            blockTextures[1] = texture; // back
        }

        if (variants.contains("bottom") && !variants["bottom"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["bottom"]["model"].get<std::string>());
            blockTextures[2] = texture; // bottom
        }

        if (variants.contains("top") && !variants["top"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["top"]["model"].get<std::string>());
            blockTextures[3] = texture; // top
        }

        if (variants.contains("left") && !variants["left"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["left"]["model"].get<std::string>());
            blockTextures[4] = texture; // left
        }

        if (variants.contains("right") && !variants["right"]["model"].is_null()) {
            sf::Texture& texture = getTexture(variants["right"]["model"].get<std::string>());
            blockTextures[5] = texture; // right
        }

        if (variants.contains("all")) {
            sf::Texture& texture = getTexture(variants["all"]["model"].get<std::string>());
            std::fill(blockTextures.begin(), blockTextures.end(), texture); // Apply the same texture for all faces
        }
    }

    return blockTextures;
}

sf::Texture& Texture::getTexture(const std::string& name) {
    if (name == "grass") return Texture::grass;
    else if (name == "grass_side") return Texture::grassSide;
    else if (name == "dirt") return Texture::dirt;
    else if (name == "stone") return Texture::stone;
    else if (name == "water") return Texture::water;
    else if (name == "planks") return Texture::planks;
    else return Texture::none;
}