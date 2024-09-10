#ifndef MINECRAFTCLONE_CAMERA_H
#define MINECRAFTCLONE_CAMERA_H


#include <SFML/OpenGL.hpp>
#include <cmath>
#include <Window.hpp>
#include <Graphics/RenderWindow.hpp>

class Camera {
public:
    Camera();

    // Update the camera based on user input
    void update(float deltaTime, sf::RenderWindow& window);

    // Apply the camera transformations (position and rotation)
    void apply() const;

    // Lock the mouse to the center of the window
    void lockMouse(sf::RenderWindow& window);

    // Unlock the mouse
    void unlockMouse(sf::RenderWindow& window);

    // Handle the escape key to unlock the mouse
    void handleEscape(sf::RenderWindow& window);

private:
    float x, y, z;      // Camera position
    float pitch, yaw;   // Camera rotation (look up/down and left/right)
    float speed;        // Movement speed

    // Sensitivity for the mouse input
    float sensitivity;

    // Whether the mouse is locked (for first person camera)
    bool isMouseLocked = false;

    void handleInput(float deltaTime);  // Handle keyboard input for movement
    void handleMouseInput(float deltaTime, sf::RenderWindow& window);  // Handle mouse input for looking around
};


#endif //MINECRAFTCLONE_CAMERA_H
