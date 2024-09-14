#ifndef MINECRAFTCLONE_TEXTURE_H
#define MINECRAFTCLONE_TEXTURE_H


#include <SFML/Graphics.hpp>
#include "../Core/Block.h"

class Texture {
public:
    static sf::Texture grass;
    static sf::Texture grassSide;
    static sf::Texture dirt;
    static sf::Texture stone;
    static sf::Texture water;
    static sf::Texture planks;
    static sf::Texture none;

    static void loadTextures();

    static std::vector<sf::Texture> initTextures(BlockType type);

    static sf::Texture& getTexture(const std::string& name);
};


#endif
