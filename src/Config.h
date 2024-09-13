#ifndef MINECRAFTCLONE_CONFIG_H
#define MINECRAFTCLONE_CONFIG_H


#include <string>
#include <Graphics/Font.hpp>
#include <Windows.h>
#include <System/Vector3.hpp>

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
        const float CROUCH_SPEED = 1.612f;

        const float NORMAL_HEIGHT = 1.72f;
        const float CROUCH_HEIGHT = 1.2f;

        const float FOV = 50.0f;

        const float SENSITIVITY = 0.1f;

        const sf::Vector3f POSITION = {0.0f, 25.0f, 0.0f};

        const float PITCH = 0.0f;
        const float YAW = 0.0f;

        const float GRAVITY = 27.55f;
        const float JUMP_VELOCITY = 8.0f;

        const float MAX_REACH = 5.0f;
    }

    namespace World {
        const int CHUNK_SIZE = 16;
        const int RENDER_DISTANCE = 1;

        const sf::Vector3f SKY_COLOR = {0.431f, 0.694f, 1.0f};

        const unsigned int SEED = 12345;
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
