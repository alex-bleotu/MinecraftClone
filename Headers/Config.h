#ifndef MINECRAFTCLONE_CONFIG_H
#define MINECRAFTCLONE_CONFIG_H


#include <string>
#include <Graphics/Font.hpp>
#include <Windows.h>

namespace Config {
    static std::string executablePath;

    namespace Window {
        const unsigned int WIDTH = 1920;
        const unsigned int HEIGHT = 1080;
        const std::string TITLE = "Minecraft Clone";

        const unsigned int FPS = 60;
    }

    namespace Widgets {
        const unsigned int TITLE_FONT_SIZE = 100;

        const unsigned int BUTTON_WIDTH = 300;
        const unsigned int BUTTON_HEIGHT = 125;

        const unsigned int FONT_SIZE = 50;
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
