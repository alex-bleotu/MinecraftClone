#ifndef MINECRAFTCLONE_CONFIG_H
#define MINECRAFTCLONE_CONFIG_H


#include <string>
#include <Graphics/Font.hpp>
#include <Windows.h>

namespace Config {
    static std::string executablePath;

    namespace Window {
        const unsigned int WIDTH = 800;
        const unsigned int HEIGHT = 600;
        const std::string TITLE = "Minecraft Clone";

        const unsigned int FPS = 60;
    }

    class Assets {
    public:
        sf::Font font;

    private:
        static Assets* assets;
        Assets();

    public:
        Assets(const Assets& other) = default;
        static Assets& get();
    };
}


#endif
