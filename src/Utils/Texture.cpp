#include <iostream>
#include <filesystem>
#include "Texture.h"
#include "../Core/Block.h"

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

std::vector<sf::Texture> Texture::initTextures(BlockType type) {
    if (type == BlockType::GRASS) {
        return {Texture::grassSide, Texture::grassSide, Texture::dirt, Texture::grass, Texture::grassSide, Texture::grassSide};
    } else if (type == BlockType::DIRT) {
        return {Texture::dirt, Texture::dirt, Texture::dirt, Texture::dirt, Texture::dirt, Texture::dirt};
    } else if (type == BlockType::STONE) {
        return {Texture::stone, Texture::stone, Texture::stone, Texture::stone, Texture::stone, Texture::stone};
    } else if (type == BlockType::WATER) {
        return {Texture::water, Texture::water, Texture::water, Texture::water, Texture::water, Texture::water};
    } else if (type == BlockType::PLANKS) {
        return {Texture::planks, Texture::planks, Texture::planks, Texture::planks, Texture::planks, Texture::planks};
    } else {
        return {Texture::none, Texture::none, Texture::none, Texture::none, Texture::none, Texture::none};
    }
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