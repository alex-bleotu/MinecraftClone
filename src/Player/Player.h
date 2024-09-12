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

    [[maybe_unused]] [[nodiscard]] sf::Vector3f getPosition() const;
    void setPosition(const sf::Vector3f& position);

    [[nodiscard]] sf::Vector3f getLookDirection() const;

    [[nodiscard]] bool getIsGrounded() const;
    [[nodiscard]] bool getIsSprinting() const;
    [[nodiscard]] bool getIsCrouching() const;
    [[nodiscard]] bool getIsFlying() const;  // Check if the player is flying

    [[nodiscard]] BlockType getCurrentBlock() const;

    [[nodiscard]] Math::AABB getAABB() const;

    std::tuple<sf::Vector3i, sf::Vector3f, sf::Vector3f> raycast(World& world) const;  // Raycast from the player's position to find the first block hit

    void breakBlock(World& world) const;                            // Break a block at the player's position
    void placeBlock(World& world, BlockType blockType) const;       // Place a block at the player's position

private:
    sf::Vector3f position;
    float pitch, yaw;

    float speed;
    const float sprintSpeed;
    const float crouchSpeed;

    const float normalHeight;
    const float crouchHeight;

    const float jumpVelocity;
    const float gravity;
    float verticalVelocity;

    bool isGrounded;   //Track if the player is grounded
    bool isSprinting;  // Track if the player is sprinting
    bool isCrouching;  // Track if the player is crouching
    bool isFlying;     // Track if the player is flying

    bool spaceHeld;    // Track if the space key is held down

    const float sensitivity;

    bool isMouseLocked = false;
    bool previousLeftMousePressed = false;
    bool previousRightMousePressed = false;

    float maxReach;

    BlockType currentBlock;

    sf::Clock spacePressClock;      // Clock to detect double-space presses
    bool spacePressedOnce = false;  // Track if space was pressed once

    void handleInput(float deltaTime, World& world);                     // Handle player input
    void handleMouseInput(float deltaTime, sf::RenderWindow& window);    // Handle mouse input
    void handleBlockChange(float deltaTime);                             // Handle block change

    void updateVerticalMovement(float deltaTime, World& world);          // Update vertical movement
    void toggleFlying();                                                 // Toggle flying mode
};

#endif // MINECRAFTCLONE_PLAYER_H
