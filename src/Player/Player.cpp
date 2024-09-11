#include "Player.h"
#include "../Config.h"

Player::Player() : x(Config::Player::X), y(Config::Player::Y), z(Config::Player::Z),
                   pitch(Config::Player::PITCH), yaw(Config::Player::YAW), speed(Config::Player::MOVE_SPEED),
                   sensitivity(Config::Player::SENSITIVITY) {}

void Player::update(float deltaTime, sf::RenderWindow& window) {
    // Handle keyboard input for movement
    handleInput(deltaTime);

    // Handle mouse input for looking around
    handleMouseInput(deltaTime, window);

    // Handle the escape key to unlock the mouse
    handleEscape(window);
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

    // Move forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        x += sin(yaw * M_PI / 180.0f) * moveSpeed;
        z -= cos(yaw * M_PI / 180.0f) * moveSpeed;
    }
    // Move backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        x -= sin(yaw * M_PI / 180.0f) * moveSpeed;
        z += cos(yaw * M_PI / 180.0f) * moveSpeed;
    }
    // Strafe left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        x -= cos(yaw * M_PI / 180.0f) * moveSpeed;
        z -= sin(yaw * M_PI / 180.0f) * moveSpeed;
    }
    // Strafe right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        x += cos(yaw * M_PI / 180.0f) * moveSpeed;
        z += sin(yaw * M_PI / 180.0f) * moveSpeed;
    }
    // Move up (Space)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        y += moveSpeed;
    }
    // Move down (Shift)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        y -= moveSpeed;
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