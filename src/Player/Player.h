#ifndef MINECRAFTCLONE_PLAYER_H
#define MINECRAFTCLONE_PLAYER_H

#include <SFML/OpenGL.hpp>
#include <cmath>
#include <Window.hpp>
#include <Graphics/RenderWindow.hpp>
#include "../Core/World.h"
#include "../Utils/Math.h"

class Player {
public:
    Player();

    void update(float deltaTime, sf::RenderWindow& window, World& world);
    void render(sf::RenderWindow& window) const;
    void apply() const;
    void lockMouse(sf::RenderWindow& window);
    void unlockMouse(sf::RenderWindow& window);
    void handleEscape(sf::RenderWindow& window);

    [[nodiscard]] sf::Vector3f getPosition() const;
    void setPosition(const sf::Vector3f& position);
    [[nodiscard]] sf::Vector3f getLookDirection() const;

    [[nodiscard]] bool getIsGrounded() const;
    [[nodiscard]] bool getIsSprinting() const;
    [[nodiscard]] bool getIsCrouching() const;
    [[nodiscard]] bool getIsFlying() const;  // NEW: Check if the player is flying

    [[nodiscard]] BlockType getCurrentBlock() const;
    [[nodiscard]] Math::AABB getAABB() const;

private:
    float x, y, z;
    float pitch, yaw;

    float speed;
    float sprintSpeed;
    float crouchSpeed;

    float normalHeight;
    float crouchHeight;

    float jumpVelocity;
    float gravity;
    float verticalVelocity;

    bool isGrounded;   //Track if the player is grounded
    bool isSprinting;  // Track if the player is sprinting
    bool isCrouching;  // Track if the player is crouching
    bool isFlying;     // Track if the player is flying

    bool spaceHeld;    // Track if the space key is held down

    float sensitivity;
    bool isMouseLocked = false;
    bool previousLeftMousePressed = false;
    bool previousRightMousePressed = false;

    BlockType currentBlock;

    sf::Clock spacePressClock;  // NEW: Clock to detect double-space presses
    bool spacePressedOnce = false;  // NEW: Track if space was pressed once

    void handleInput(float deltaTime, World& world);
    void handleMouseInput(float deltaTime, sf::RenderWindow& window, World& world);
    void handleBlockChange(float deltaTime);

    void updateVerticalMovement(float deltaTime, World& world);
    void toggleFlying();  // NEW: Toggle flying mode
};

#endif // MINECRAFTCLONE_PLAYER_H
