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
    void update(float deltaTime, sf::RenderWindow& window, World& world);

    // Render the player (for debugging purposes)
    void render(sf::RenderWindow& window) const;

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

    // Check if the player is colliding with any blocks in the world
    [[nodiscard]] sf::Vector3f getLookDirection() const;

    bool getIsGrounded() const;   // Get the player's grounded status
    bool getIsSprinting() const;  // Get the player's sprinting status
    bool getIsCrouching() const;  // Get the player's crouching status

    // Get the player's current block
    [[nodiscard]] BlockType getCurrentBlock() const;

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

    // Previous mouse position for calculating the mouse delta
    bool previousLeftMousePressed = false;
    // Previous right mouse position for calculating the mouse delta
    bool previousRightMousePressed = false;

    BlockType currentBlock;  // The current block the player is holding

    void handleInput(float deltaTime, World& world);  // Handle keyboard input for movement and jumping
    void handleMouseInput(float deltaTime, sf::RenderWindow& window);  // Handle mouse input for looking around
    void handleBlockChange(float deltaTime);  // Handle block change input

    // Update the player's vertical movement (jumping and gravity)
    void updateVerticalMovement(float deltaTime, World& world);

    // Check if the player is colliding with any blocks in the world
    [[nodiscard]] Math::AABB getPlayerAABB() const;
};

#endif //MINECRAFTCLONE_PLAYER_H
