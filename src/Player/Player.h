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

    // Update the player based on user input
    void update(float deltaTime, sf::RenderWindow& window, const World& world);

    // Apply the player transformations (position and rotation)
    void apply() const;

    // Lock the mouse to the center of the window
    void lockMouse(sf::RenderWindow& window);

    // Unlock the mouse
    void unlockMouse(sf::RenderWindow& window);

    // Handle the escape key to unlock the mouse
    void handleEscape(sf::RenderWindow& window);

    // Get the player position
    [[nodiscard]] sf::Vector3f getPosition() const;

    // Set the player position
    void setPosition(const sf::Vector3f& position);

private:
    float x, y, z;      // Player position
    float pitch, yaw;   // Player rotation (look up/down and left/right)

    float speed;        // Movement speed
    float sprintSpeed;  // Sprinting speed (increased movement speed when sprinting)
    float crouchSpeed;  // Crouching speed (reduced movement speed when crouching)

    float normalHeight; // Normal height of the player
    float crouchHeight; // Crouching height of the player

    float jumpVelocity;     // Velocity during jump
    float gravity;          // Gravity strength
    float verticalVelocity; // Current velocity in the y-axis

    bool isGrounded;    // Is the player on the ground?
    bool isSprinting;   // Is the player sprinting?
    bool isCrouching;   // Is the player crouching?

    // Sensitivity for the mouse input
    float sensitivity;

    // Whether the mouse is locked (for first person player)
    bool isMouseLocked = false;

    void handleInput(float deltaTime, const World& world);  // Handle keyboard input for movement and jumping
    void handleMouseInput(float deltaTime, sf::RenderWindow& window);  // Handle mouse input for looking around

    // Update the player's vertical movement (jumping and gravity)
    void updateVerticalMovement(float deltaTime, const World& world);

    // Check if the player is colliding with any blocks in the world
    [[nodiscard]] Math::AABB getPlayerAABB() const;
};

#endif //MINECRAFTCLONE_PLAYER_H
