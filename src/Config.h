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

        const unsigned int DEPTH_BITS = 24;

        const unsigned int FPS = 60;
    }

    namespace Widgets {
        const unsigned int TITLE_FONT_SIZE = 100;

        const float BUTTON_WIDTH = 300;
        const float BUTTON_HEIGHT = 125;

        const unsigned int FONT_SIZE = 50;
    }

    namespace Player {
        const float MOVE_SPEED = 5.0f;
        const float SENSITIVITY = 0.1f;

        const float X = 0.0f;
        const float Y = 5.0f;
        const float Z = 0.0f;

        const float PITCH = 30.0f;
        const float YAW = 135.0f;

        const float GRAVITY = -9.8f;
        const float JUMP_STRENGTH = 5.0f;
        const float GROUND_LEVEL = 0.0f;
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
