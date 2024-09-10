#include <utility>
#include <valarray>
#include <iostream>
#include "Block.h"
#include "../Utils/Math.h"
#include "../Config.h"

// Define static vertices for the cube
const GLfloat Block::vertices[24] = {
        -0.5f, -0.5f, -0.5f,  // Vertex 0
        0.5f, -0.5f, -0.5f,  // Vertex 1
        0.5f,  0.5f, -0.5f,  // Vertex 2
        -0.5f,  0.5f, -0.5f,  // Vertex 3
        -0.5f, -0.5f,  0.5f,  // Vertex 4
        0.5f, -0.5f,  0.5f,  // Vertex 5
        0.5f,  0.5f,  0.5f,  // Vertex 6
        -0.5f,  0.5f,  0.5f   // Vertex 7
};

// Define static indices for the cube
const GLuint Block::indices[36] = {
        0, 1, 2, 2, 3, 0,  // Front face
        4, 5, 6, 6, 7, 4,  // Back face
        0, 1, 5, 5, 4, 0,  // Bottom face
        2, 3, 7, 7, 6, 2,  // Top face
        0, 3, 7, 7, 4, 0,  // Left face
        1, 2, 6, 6, 5, 1   // Right face
};

// Constructor to initialize block with type and position
Block::Block(BlockType type, const sf::Vector3i& position)
        : m_type(type), m_position(position) {
    // If the block is AIR, it's not visible
    m_isVisible = (type != BlockType::AIR);
}

// Getter for the block type
BlockType Block::getType() const {
    return m_type;
}

// Setter for the block type
void Block::setType(BlockType type) {
    m_type = type;
    // Recalculate visibility
    m_isVisible = (m_type != BlockType::AIR);
}

// Getter for the block position
sf::Vector3i Block::getPosition() const {
    return m_position;
}

// Setter for the block position
void Block::setPosition(const sf::Vector3i& position) {
    m_position = position;
}

// Check if block is visible (i.e., not air)
bool Block::isVisible() const {
    return m_isVisible;
}

// Render the block
void Block::render() const {
    if (!m_isVisible) return; // Only render if the block is visible

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the color based on block type
    switch (m_type) {
        case BlockType::DIRT:
            glColor3f(0.545f, 0.271f, 0.075f); // Brown for dirt
            break;
        case BlockType::GRASS:
            glColor3f(0.133f, 0.545f, 0.133f); // Green for grass
            break;
        case BlockType::STONE:
            glColor3f(0.5f, 0.5f, 0.5f); // Gray for stone
            break;
        case BlockType::WATER:
            glColor3f(0.0f, 0.0f, 1.0f); // Blue for water
            break;
        default:
            glColor3f(1.0f, 1.0f, 1.0f); // Default white
            break;
    }

    // Save the current matrix state
    glPushMatrix();

    // Translate to the block's position
    glTranslatef(static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_position.z));

    // Render the cube
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 36; ++i) {
        int vertexIndex = indices[i];
        glVertex3f(vertices[3 * vertexIndex], vertices[3 * vertexIndex + 1], vertices[3 * vertexIndex + 2]);
    }
    glEnd();

    // Restore the previous matrix state
    glPopMatrix();

    // Disable depth testing after rendering
    glDisable(GL_DEPTH_TEST);
}

