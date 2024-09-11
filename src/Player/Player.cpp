#include <iostream>
#include "Player.h"
#include "../Config.h"

Player::Player() : x(Config::Player::X), y(Config::Player::Y), z(Config::Player::Z),
                   pitch(Config::Player::PITCH), yaw(Config::Player::YAW), speed(Config::Player::MOVE_SPEED),
                   sprintSpeed(Config::Player::SPRINT_SPEED), crouchSpeed(Config::Player::CROUCH_SPEED),
                   normalHeight(Config::Player::NORMAL_HEIGHT), crouchHeight(Config::Player::CROUCH_HEIGHT),
                   sensitivity(Config::Player::SENSITIVITY), gravity(Config::Player::GRAVITY),
                   jumpVelocity(Config::Player::JUMP_VELOCITY), verticalVelocity(0.0f),
                   isGrounded(false), isSprinting(false), isCrouching(false) {}

void Player::update(float deltaTime, sf::RenderWindow& window, const World& world) {
    // Handle keyboard input for movement and jumping
    handleInput(deltaTime, world);  // Pass the world for collision checks

    // Handle mouse input for looking around
    handleMouseInput(deltaTime, window);

    // Handle the escape key to unlock the mouse
    handleEscape(window);

    // Update vertical movement (gravity and jumping)
    updateVerticalMovement(deltaTime, world);  // Pass the world for collision checks
}

void Player::apply() const {
    // Reset the matrix and apply the player transformations
    glLoadIdentity();

    // Rotate based on yaw (left/right) and pitch (up/down)
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);  // Looking up/down
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);    // Looking left/right

    // Move the player to its position
    // Adjust camera height based on whether the player is crouching
    float cameraHeight = isCrouching ? crouchHeight : normalHeight - 0.1f;
    glTranslatef(-x, -(y + cameraHeight), -z);
}

void Player::handleInput(float deltaTime, const World& world) {
    float moveSpeed = speed * deltaTime;
    float moveX = 0.0f, moveZ = 0.0f;  // Initialize movement on the X and Z axes

    // Sprinting increases movement speed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        isSprinting = true;
        moveSpeed = sprintSpeed * deltaTime;  // Increase movement speed while sprinting
    } else {
        isSprinting = false;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        if (!isCrouching) {
            isCrouching = true;
            speed = crouchSpeed;  // Reduce movement speed when crouching
        }
    } else {
        if (isCrouching) {
            isCrouching = false;
            speed = Config::Player::MOVE_SPEED;  // Reset to normal speed
        }
    }

    // Move forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        moveX += sin(yaw * M_PI / 180.0f);
        moveZ -= cos(yaw * M_PI / 180.0f);
    }
    // Move backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        moveX -= sin(yaw * M_PI / 180.0f);
        moveZ += cos(yaw * M_PI / 180.0f);
    }
    // Strafe left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        moveX -= cos(yaw * M_PI / 180.0f);
        moveZ -= sin(yaw * M_PI / 180.0f);
    }
    // Strafe right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        moveX += cos(yaw * M_PI / 180.0f);
        moveZ += sin(yaw * M_PI / 180.0f);
    }

    // Normalize the movement vector if there's movement
    float length = std::sqrt(moveX * moveX + moveZ * moveZ);
    if (length > 0.0f) {
        moveX /= length;
        moveZ /= length;
    }

    // New potential position based on input
    sf::Vector3f newPosition = { x + moveX * moveSpeed, y, z + moveZ * moveSpeed };

    // Sliding mechanic: Handle collisions on X and Z axes independently
    Math::AABB playerAABB = getPlayerAABB();  // Current player's AABB
    playerAABB.min.x += moveX * moveSpeed;
    playerAABB.max.x += moveX * moveSpeed;

    // First, check collision on X axis
    if (!world.checkCollision(playerAABB)) {
        x = newPosition.x;  // No collision on X axis, apply movement
    } else {
        std::cout << "Collision on X axis, sliding along Z!" << std::endl;
    }

    // Now check the Z axis movement
    playerAABB = getPlayerAABB();  // Reset to current AABB
    playerAABB.min.z += moveZ * moveSpeed;
    playerAABB.max.z += moveZ * moveSpeed;

    if (!world.checkCollision(playerAABB)) {
        z = newPosition.z;  // No collision on Z axis, apply movement
    } else {
        std::cout << "Collision on Z axis, sliding along X!" << std::endl;
    }

    // Jumping (only when grounded)
    if (isGrounded && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        verticalVelocity = jumpVelocity;
        isGrounded = false;
    }
}

void Player::handleMouseInput(float deltaTime, sf::RenderWindow& window) {
    if (!isMouseLocked) return;

    // Get the center of the window
    sf::Vector2u windowSize = { Config::Window::WIDTH, Config::Window::HEIGHT };
    sf::Vector2i center(windowSize.x / 2, windowSize.y / 2);

    // Get the current mouse position relative to the window
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Calculate the offset (how much the mouse moved)
    float offsetX = (mousePos.x - center.x) * sensitivity;
    float offsetY = (mousePos.y - center.y) * sensitivity;

    // Adjust yaw and pitch based on the mouse movement
    yaw += offsetX;
    pitch += offsetY;

    // Clamp the pitch so the player doesn't flip upside down
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Reset the mouse position to the center of the window
    sf::Mouse::setPosition(center, window);
}

void Player::lockMouse(sf::RenderWindow& window) {
    // Lock the mouse to the center of the window
    sf::Vector2u windowSize = { Config::Window::WIDTH, Config::Window::HEIGHT };
    sf::Vector2i center(windowSize.x / 2, windowSize.y / 2);

    sf::Mouse::setPosition(center, window);
    isMouseLocked = true;

    window.setMouseCursorVisible(false);
}

void Player::unlockMouse(sf::RenderWindow& window) {
    isMouseLocked = false;

    window.setMouseCursorVisible(true);
}

void Player::handleEscape(sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        unlockMouse(window);
    }
}

sf::Vector3f Player::getPosition() const {
    return { x, y, z };
}

void Player::setPosition(const sf::Vector3f& position) {
    x = position.x;
    y = position.y;
    z = position.z;
}

void Player::updateVerticalMovement(float deltaTime, const World& world) {
    if (!isGrounded || verticalVelocity != 0) {  // Handle movement when jumping or falling
        // Apply gravity if the player is not grounded
        verticalVelocity -= gravity * deltaTime;
        sf::Vector3f newPosition = { x, y + verticalVelocity * deltaTime, z };

        // Check for collision in the vertical direction
        Math::AABB playerAABB = getPlayerAABB();
        playerAABB.min.y += verticalVelocity * deltaTime;
        playerAABB.max.y += verticalVelocity * deltaTime;

        if (!world.checkCollision(playerAABB)) {
            // No collision, update Y position
            y = newPosition.y;
            isGrounded = false;  // Player is not grounded if there is no collision
        } else {
            if (verticalVelocity > 0) {  // Collided with ceiling while moving up
                verticalVelocity = 0.0f;  // Stop upward movement
            } else {  // Collided with ground while falling
                verticalVelocity = 0.0f;  // Stop falling
                isGrounded = true;  // Player is now grounded
            }
        }
    }

    // Re-check if the player is still grounded after horizontal movement
    Math::AABB playerAABB = getPlayerAABB();
    playerAABB.min.y -= 0.1f;  // Check slightly below the player's feet
    if (!world.checkCollision(playerAABB)) {
        isGrounded = false;  // If there is no block below the player, set grounded to false
    }
}

Math::AABB Player::getPlayerAABB() const {
    if (isCrouching) {
        return Math::AABB({ x - 0.3f, y, z - 0.3f }, { x + 0.3f, y + crouchHeight, z + 0.3f });
    } else {
        return Math::AABB({ x - 0.3f, y, z - 0.3f }, { x + 0.3f, y + normalHeight, z + 0.3f });
    }
}
