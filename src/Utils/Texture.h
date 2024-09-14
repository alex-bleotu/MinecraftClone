#ifndef MINECRAFTCLONE_TEXTURE_H
#define MINECRAFTCLONE_TEXTURE_H


#include <SFML/Graphics.hpp>
#include "../Core/Block.h"

class Texture {
public:
    static sf::Texture atlas;

    static void loadTextures();

    static std::pair<std::vector<sf::IntRect>, std::vector<int>> initTextures(BlockType type);

    static sf::IntRect getTextureCoords(const std::string& name);
};


#endif
