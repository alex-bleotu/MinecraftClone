#include <iostream>
#include "Player.h"
#include "../Config.h"

Player::Player() : x(Config::Player::X), y(Config::Player::Y), z(Config::Player::Z),
                   pitch(Config::Player::PITCH), yaw(Config::Player::YAW), speed(Config::Player::MOVE_SPEED),
                   sprintSpeed(Config::Player::SPRINT_SPEED), crouchSpeed(Config::Player::CROUCH_SPEED),
                   normalHeight(Config::Player::NORMAL_HEIGHT), crouchHeight(Config::Player::CROUCH_HEIGHT),
                   sensitivity(Config::Player::SENSITIVITY), gravity(Config::Player::GRAVITY),
                   jumpVelocity(Config::Player::JUMP_VELOCITY), verticalVelocity(0.0f),
                   isGrounded(false), isSprinting(false), isCrouching(false), currentBlock(BlockType::PLANKS),
                   spaceHeld(false) {}

void Player::update(float deltaTime, sf::RenderWindow& window, World& world) {
    // Handle keyboard input for movement and jumping
    handleInput(deltaTime, world);

    // Handle mouse input for looking around
    handleMouseInput(deltaTime, window, world);

    // Handle the escape key to unlock the mouse
    handleEscape(window);

    // Update vertical movement (gravity and jumping or flying)
    updateVerticalMovement(deltaTime, world);

    // Handle block change input
    handleBlockChange(deltaTime);
}

void Player::render(sf::RenderWindow& window) const {
    // Get the window size
    sf::Vector2u windowSize = window.getSize();

    // Center of the window
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;

    // Define the crosshair size (in pixels)
    float crosshairSize = 10.0f;

    // Set up OpenGL to render 2D lines
    glPushMatrix();  // Save current matrix state

    // Switch to 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();  // Save the projection matrix
    glLoadIdentity();  // Reset the projection matrix
    glOrtho(0, windowSize.x, windowSize.y, 0, -1, 1);  // Set up orthogonal projection

    // Set the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();  // Reset the modelview matrix

    // Disable depth testing so the crosshair renders on top
    glDisable(GL_DEPTH_TEST);

    // Set the color of the crosshair (white in this case)
    glColor3f(1.0f, 1.0f, 1.0f);

    // Render the vertical line of the crosshair
    glBegin(GL_LINES);
    glVertex2f(centerX, centerY - crosshairSize);  // Top of the vertical line
    glVertex2f(centerX, centerY + crosshairSize);  // Bottom of the vertical line
    glEnd();

    // Render the horizontal line of the crosshair
    glBegin(GL_LINES);
    glVertex2f(centerX - crosshairSize, centerY);  // Left end of the horizontal line
    glVertex2f(centerX + crosshairSize, centerY);  // Right end of the horizontal line
    glEnd();

    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);

    // Restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // Restore previous projection matrix

    // Restore the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();  // Restore previous modelview matrix
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

void Player::handleInput(float deltaTime, World& world) {
    float moveSpeed = isFlying ? speed * deltaTime * 2 : speed * deltaTime;  // Double speed when flying
    float moveX = 0.0f, moveZ = 0.0f;

    // Handle sprinting
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
        isSprinting = true;
        moveSpeed = sprintSpeed * deltaTime;
    } else {
        isSprinting = false;
    }

    // Handle crouching, but only if the player is not flying
    if (!isFlying) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            if (!isCrouching) {
                isCrouching = true;
                speed = crouchSpeed;
            }
        } else {
            if (isCrouching) {
                isCrouching = false;
                speed = Config::Player::MOVE_SPEED;  // Reset speed when not crouching
            }
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

    // Handle double-space for flying toggle
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (!spacePressedOnce) {
            // First space press detected
            spacePressedOnce = true;
            spacePressClock.restart();
        } else if (spacePressClock.getElapsedTime().asMilliseconds() < 200 && !spaceHeld) {
            // Second space press within 200ms, toggle flying
            toggleFlying();
            spacePressedOnce = false;  // Reset the detection
        }
        spaceHeld = true;
    } else {
        // Reset single press if the time limit has passed without a second press
        if (spacePressedOnce && spacePressClock.getElapsedTime().asMilliseconds() >= 200) {
            spacePressedOnce = false;
        }
        spaceHeld = false;
    }

    // Movement when flying or walking
    sf::Vector3f newPosition = { x + moveX * moveSpeed, y, z + moveZ * moveSpeed };

    // Handle collision detection for X and Z axes (horizontal movement)
    Math::AABB playerAABB = getAABB();
    playerAABB.min.x += moveX * moveSpeed;
    playerAABB.max.x += moveX * moveSpeed;

    if (!world.checkCollision(playerAABB)) {
        x = newPosition.x;  // Allow movement on X axis if no collision
    }

    playerAABB = getAABB();  // Reset AABB
    playerAABB.min.z += moveZ * moveSpeed;
    playerAABB.max.z += moveZ * moveSpeed;

    if (!world.checkCollision(playerAABB)) {
        z = newPosition.z;  // Allow movement on Z axis if no collision
    }


    // Check for left mouse button press (for block breaking)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!previousLeftMousePressed) {
            // Left button was just pressed (single press)
            world.breakBlock(*this);
        }
        previousLeftMousePressed = true;  // Update state to "pressed"
    } else {
        previousLeftMousePressed = false;  // Update state to "not pressed"
    }

    // Check for right mouse button press (for block placing)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (!previousRightMousePressed) {
            // Right button was just pressed (single press)
            world.placeBlock(*this, currentBlock);
        }
        previousRightMousePressed = true;  // Update state to "pressed"
    } else {
        previousRightMousePressed = false;  // Update state to "not pressed"
    }

    if (isFlying) {
        // Handle vertical movement when flying
        float moveY = 0.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            moveY = moveSpeed;  // Move up
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            moveY = -moveSpeed;  // Move down
        }

        newPosition = { x, y + moveY, z };  // New position considering Y movement

        // Handle collision detection for Y axis (vertical movement)
        playerAABB = getAABB();
        playerAABB.min.y += moveY;
        playerAABB.max.y += moveY;

        if (!world.checkCollision(playerAABB)) {
            y = newPosition.y;  // Apply movement on Y axis if no collision
        }
    } else {
        // Jumping (only when grounded and not flying)
        if (isGrounded && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            verticalVelocity = jumpVelocity;
            isGrounded = false;
        }
    }
}


void Player::toggleFlying() {
    isFlying = !isFlying;
    if (isFlying) {
        verticalVelocity = 0.0f;  // Reset vertical velocity when flying
    } else {
        isGrounded = false;  // Ensure grounded is false when exiting flying mode
    }
}

void Player::handleBlockChange(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        currentBlock = BlockType::GRASS;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        currentBlock = BlockType::DIRT;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        currentBlock = BlockType::STONE;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        currentBlock = BlockType::PLANKS;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        currentBlock = BlockType::WATER;
    }
}

void Player::handleMouseInput(float deltaTime, sf::RenderWindow& window, World& world) {
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

void Player::updateVerticalMovement(float deltaTime, World& world) {
    if (isFlying) return;  // Skip gravity/jumping logic when flying

    if (!isGrounded || verticalVelocity != 0) {
        verticalVelocity -= gravity * deltaTime;
        sf::Vector3f newPosition = { x, y + verticalVelocity * deltaTime, z };

        Math::AABB playerAABB = getAABB();
        playerAABB.min.y += verticalVelocity * deltaTime;
        playerAABB.max.y += verticalVelocity * deltaTime;

        if (!world.checkCollision(playerAABB)) {
            y = newPosition.y;
            isGrounded = false;
        } else {
            if (verticalVelocity > 0) {
                verticalVelocity = 0.0f;
            } else {
                verticalVelocity = 0.0f;
                isGrounded = true;
            }
        }
    }

    Math::AABB playerAABB = getAABB();
    playerAABB.min.y -= 0.1f;
    if (!world.checkCollision(playerAABB)) {
        isGrounded = false;
    }
}

bool Player::getIsFlying() const {
    return isFlying;
}

Math::AABB Player::getAABB() const {
    if (isCrouching) {
        return {{ x - 0.3f, y, z - 0.3f }, { x + 0.3f, y + crouchHeight, z + 0.3f }};
    } else {
        return {{ x - 0.3f, y, z - 0.3f }, { x + 0.3f, y + normalHeight, z + 0.3f }};
    }
}

sf::Vector3f Player::getLookDirection() const {
    // Convert yaw and pitch from degrees to radians
    float yawRadians = yaw * M_PI / 180.0f;
    float pitchRadians = -pitch * M_PI / 180.0f;

    // Calculate the look direction based on yaw and pitch
    sf::Vector3f lookDirection;
    lookDirection.x = cos(pitchRadians) * sin(yawRadians);   // Left-right direction
    lookDirection.y = sin(pitchRadians);                     // Up-down direction
    lookDirection.z = cos(pitchRadians) * -cos(yawRadians);  // Forward-backward direction

    return lookDirection;
}

bool Player::getIsGrounded() const {
    return isGrounded;
}

bool Player::getIsSprinting() const {
    return isSprinting;
}

bool Player::getIsCrouching() const {
    return isCrouching;
}

BlockType Player::getCurrentBlock() const {
    return currentBlock;
}
