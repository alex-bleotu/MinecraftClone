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
        const float MOVE_SPEED = 4.317f;
        const float SPRINT_SPEED = 5.612f;
        const float JUMPING_SPRINT_SPEED = 7.127f;

        const float SENSITIVITY = 0.1f;

        const float X = 0.0f;
        const float Y = 2.0f;
        const float Z = 0.0f;

        const float PITCH = 0.0f;
        const float YAW = 0.0f;

        const float GRAVITY = 27.55f;
        const float JUMP_VELOCITY = 8.0f;
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
