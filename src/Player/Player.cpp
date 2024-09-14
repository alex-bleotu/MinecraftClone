#include <iostream>
#include "Player.h"
#include "../Config.h"

Player::Player() : position(Config::Player::POSITION),
                   pitch(Config::Player::PITCH), yaw(Config::Player::YAW), speed(Config::Player::MOVE_SPEED),
                   sprintSpeed(Config::Player::SPRINT_SPEED), crouchSpeed(Config::Player::CROUCH_SPEED),
                   normalHeight(Config::Player::NORMAL_HEIGHT), crouchHeight(Config::Player::CROUCH_HEIGHT),
                   sensitivity(Config::Player::SENSITIVITY), gravity(Config::Player::GRAVITY),
                   jumpVelocity(Config::Player::JUMP_VELOCITY), verticalVelocity(0.0f), maxReach(Config::Player::MAX_REACH),
                   isGrounded(false), isSprinting(false), isCrouching(false), currentBlock(BlockType::PLANKS),
                   spaceHeld(false) {}

void Player::update(float deltaTime, sf::RenderWindow& window, World& world) {
    // Handle keyboard input for movement and jumping
    handleInput(deltaTime, world);

    // Handle mouse input for looking around
    handleMouseInput(deltaTime, window);

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
    glTranslatef(-position.x, -(position.y + cameraHeight), -position.z);
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
    sf::Vector3f newPosition = { position.x + moveX * moveSpeed, position.y, position.z + moveZ * moveSpeed };

    // Handle collision detection for X and Z axes (horizontal movement)
    Math::AABB playerAABB = getAABB();
    playerAABB.min.x += moveX * moveSpeed;
    playerAABB.max.x += moveX * moveSpeed;

    if (!world.checkCollision(playerAABB)) {
        position.x = newPosition.x;  // Allow movement on X axis if no collision
    }

    playerAABB = getAABB();  // Reset AABB
    playerAABB.min.z += moveZ * moveSpeed;
    playerAABB.max.z += moveZ * moveSpeed;

    if (!world.checkCollision(playerAABB)) {
        position.z = newPosition.z;  // Allow movement on Z axis if no collision
    }


    // Check for left mouse button press (for block breaking)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!previousLeftMousePressed) {
            // Left button was just pressed (single press)
            breakBlock(world);
        }
        previousLeftMousePressed = true;  // Update state to "pressed"
    } else {
        previousLeftMousePressed = false;  // Update state to "not pressed"
    }

    // Check for right mouse button press (for block placing)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (!previousRightMousePressed) {
            // Right button was just pressed (single press)
            placeBlock(world, currentBlock);
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

        newPosition = { position.x, position.y + moveY, position.z };  // New position considering Y movement

        // Handle collision detection for Y axis (vertical movement)
        playerAABB = getAABB();
        playerAABB.min.y += moveY;
        playerAABB.max.y += moveY;

        if (!world.checkCollision(playerAABB)) {
            position.y = newPosition.y;  // Apply movement on Y axis if no collision
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

    // Wrap the yaw value to keep it between 0 and 360 degrees
    if (yaw > 360.0f) yaw -= 360.0f;
    if (yaw < 0.0f) yaw += 360.0f;

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
    return position;
}

void Player::setPosition(const sf::Vector3f& position) {
    this->position = position;
}

void Player::updateVerticalMovement(float deltaTime, World& world) {
    if (isFlying) return;  // Skip gravity/jumping logic when flying

    if (!isGrounded || verticalVelocity != 0) {
        verticalVelocity -= gravity * deltaTime;
        sf::Vector3f newPosition = { position.x, position.y + verticalVelocity * deltaTime, position.z };

        Math::AABB playerAABB = getAABB();
        playerAABB.min.y += verticalVelocity * deltaTime;
        playerAABB.max.y += verticalVelocity * deltaTime;

        if (!world.checkCollision(playerAABB)) {
            position.y = newPosition.y;
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
        return {{ position.x - 0.3f, position.y, position.z - 0.3f }, { position.x + 0.3f, position.y + crouchHeight, position.z + 0.3f }};
    } else {
        return {{ position.x - 0.3f, position.y, position.z - 0.3f }, { position.x + 0.3f, position.y + normalHeight, position.z + 0.3f }};
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

void Player::breakBlock(World& world) const {
    // Raycast to find the block the player is looking at
    auto [blockPos, hitPoint, hitNormal] = raycast(world);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Remove the block at the position returned by the raycast
        world.removeBlockAt(blockPos);
    }
}


void Player::placeBlock(World& world, BlockType blockType) const {
    // Raycast to find the block the player is looking at
    auto [blockPos, hitPoint, hitNormal] = raycast(world);  // Assume max reach distance is 5 units

    if (blockPos != sf::Vector3i(-1000, -1000, -1000)) {
        // Find the adjacent position to place the block based on hitNormal
        sf::Vector3i newBlockPos = blockPos + sf::Vector3i(hitNormal.x, hitNormal.y, hitNormal.z);

        // Create the new block's AABB (assuming block size is 1x1x1)
        Math::AABB blockAABB = {
                { float(newBlockPos.x), float(newBlockPos.y), float(newBlockPos.z) },
                { float(newBlockPos.x + 1), float(newBlockPos.y + 1), float(newBlockPos.z + 1) }
        };

        // Check if the player's AABB collides with the new block's AABB
        if (!getAABB().intersects(blockAABB)) {
            // If no collision, place the block
            world.setBlockAt(newBlockPos, blockType);
        }
    }
}

std::tuple<sf::Vector3i, sf::Vector3f, sf::Vector3f> Player::raycast(World& world) const {
    sf::Vector3f rayOrigin = position;
    rayOrigin.y += isCrouching ? Config::Player::CROUCH_HEIGHT : Config::Player::NORMAL_HEIGHT;
    rayOrigin.y -= 0.1f;  // Adjust the ray origin to start at eye level

    sf::Vector3f rayDirection = getLookDirection();
    rayDirection /= std::sqrt(rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z);  // Normalize the direction vector

    // Initialize the block position (in global block coordinates)
    sf::Vector3i blockPos(std::floor(rayOrigin.x), std::floor(rayOrigin.y), std::floor(rayOrigin.z));

    // Step values for stepping through the grid in the x, y, and z directions
    sf::Vector3f tMax, tDelta, step;

    // X-axis stepping
    if (rayDirection.x > 0) {
        step.x = 1;
        tMax.x = (blockPos.x + 1 - rayOrigin.x) / rayDirection.x;
        tDelta.x = 1.0f / rayDirection.x;
    } else {
        step.x = -1;
        tMax.x = (rayOrigin.x - blockPos.x) / -rayDirection.x;
        tDelta.x = 1.0f / -rayDirection.x;
    }

    // Y-axis stepping
    if (rayDirection.y > 0) {
        step.y = 1;
        tMax.y = (blockPos.y + 1 - rayOrigin.y) / rayDirection.y;
        tDelta.y = 1.0f / rayDirection.y;
    } else {
        step.y = -1;
        tMax.y = (rayOrigin.y - blockPos.y) / -rayDirection.y;
        tDelta.y = 1.0f / -rayDirection.y;
    }

    // Z-axis stepping
    if (rayDirection.z > 0) {
        step.z = 1;
        tMax.z = (blockPos.z + 1 - rayOrigin.z) / rayDirection.z;
        tDelta.z = 1.0f / rayDirection.z;
    } else {
        step.z = -1;
        tMax.z = (rayOrigin.z - blockPos.z) / -rayDirection.z;
        tDelta.z = 1.0f / -rayDirection.z;
    }

    // Traverse the grid to find the first block hit or exceed max distance
    for (float distance = 0.0f; distance < maxReach; ) {
        sf::Vector3f hitNormal(0.0f, 0.0f, 0.0f);

        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            blockPos.x += step.x;
            distance = tMax.x;
            tMax.x += tDelta.x;
            hitNormal = { -step.x, 0.0f, 0.0f };
        } else if (tMax.y < tMax.z) {
            blockPos.y += step.y;
            distance = tMax.y;
            tMax.y += tDelta.y;
            hitNormal = { 0.0f, -step.y, 0.0f };
        } else {
            blockPos.z += step.z;
            distance = tMax.z;
            tMax.z += tDelta.z;
            hitNormal = { 0.0f, 0.0f, -step.z };
        }

        // Check if the block at the current block position is solid (or visible)
        if (const Block* block = world.getBlockAt(blockPos)) {
            if (block->checkIfVisible()) {
                sf::Vector3f hitPoint = rayOrigin + rayDirection * distance;
                return { blockPos, hitPoint, hitNormal };  // Return the block hit and hit details
            }
        }
    }

    // No block hit within the max distance
    return { sf::Vector3i(-1000, -1000, -1000), sf::Vector3f(-1000.0f, -1000.0f, -1000.0f), sf::Vector3f(0.0f, 0.0f, 0.0f) };
}