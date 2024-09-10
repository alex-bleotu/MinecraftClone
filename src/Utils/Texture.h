#ifndef MINECRAFTCLONE_TEXTURE_H
#define MINECRAFTCLONE_TEXTURE_H


#include <SFML/Graphics.hpp>

class Texture {
public:
    static sf::Texture grass;
    static sf::Texture dirt;
    static sf::Texture stone;
    static sf::Texture water;
    static sf::Texture planks;
    static sf::Texture none;

    static void loadTextures();
};


#endif
