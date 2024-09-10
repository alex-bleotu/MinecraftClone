#include <iostream>
#include <filesystem>
#include "Texture.h"

// Define the static variables
sf::Texture Texture::grass;
sf::Texture Texture::dirt;
sf::Texture Texture::stone;
sf::Texture Texture::water;
sf::Texture Texture::planks;
sf::Texture Texture::none;

void Texture::loadTextures() {
    std::string path = "assets/textures/";

    Texture::grass.loadFromFile(path + "grass.png");
    Texture::dirt.loadFromFile(path + "dirt.png");
    Texture::stone.loadFromFile(path + "stone.png");
    Texture::water.loadFromFile(path + "water.png");
    Texture::planks.loadFromFile(path + "planks.png");
    Texture::none.loadFromFile(path + "none.png");
}