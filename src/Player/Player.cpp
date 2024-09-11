#include <iostream>
#include "Player.h"
#include "../Config.h"

Player::Player() : x(Config::Player::X), y(Config::Player::Y), z(Config::Player::Z),
                   pitch(Config::Player::PITCH), yaw(Config::Player::YAW), speed(Config::Player::MOVE_SPEED),
                   sprintSpeed(Config::Player::SPRINT_SPEED), sensitivity(Config::Player::SENSITIVITY),
                   gravity(Config::Player::GRAVITY), jumpVelocity(Config::Player::JUMP_VELOCITY),
                   verticalVelocity(0.0f), isGrounded(true), isSprinting(false) {}

void Player::update(float deltaTime, sf::RenderWindow& window) {
    // Handle keyboard input for movement and jumping
    handleInput(deltaTime);

    // Handle mouse input for looking around
    handleMouseInput(deltaTime, window);

    // Handle the escape key to unlock the mouse
    handleEscape(window);

    // Update vertical movement (gravity and jumping)
    updateVerticalMovement(deltaTime);
}

void Player::apply() const {
    // Reset the matrix and apply the player transformations
    glLoadIdentity();

    // Rotate based on yaw (left/right) and pitch (up/down)
    glRotatef(pitch, 1.0f, 0.0f, 0.0f);  // Looking up/down
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);    // Looking left/right

    // Move the player to its position
    glTranslatef(-x, -y, -z);
}

void Player::handleInput(float deltaTime) {
    float moveSpeed = speed * deltaTime;
    float moveX = 0.0f, moveZ = 0.0f;  // Initialize movement on the X and Z axes

    // Sprinting increases movement speed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        isSprinting = true;
        moveSpeed = sprintSpeed * deltaTime;  // Increase movement speed while sprinting
    } else {
        isSprinting = false;
    }

    // Check if the player is sprinting and jumping
    if (isSprinting && !isGrounded) {
        // Set the specific speed to 7.127 units/second for sprinting and jumping
        moveSpeed = Config::Player::JUMPING_SPRINT_SPEED * deltaTime;
    }

    // Print current movement speed for debugging
    std::cout << "Current speed: " << moveSpeed / deltaTime << " units/second" << std::endl;

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

    // Apply the normalized movement
    x += moveX * moveSpeed;
    z += moveZ * moveSpeed;

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

void Player::updateVerticalMovement(float deltaTime) {
    if (!isGrounded) {
        // Apply gravity if the player is not grounded
        verticalVelocity -= gravity * deltaTime;
        y += verticalVelocity * deltaTime;

        // Check if the player hit the ground
        if (y <= 2.0f) {
            y = 2.0f;
            verticalVelocity = 0.0f;
            isGrounded = true;
        }
    }
}
