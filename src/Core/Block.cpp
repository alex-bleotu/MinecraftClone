#include "Block.h"
#include "../Utils/Math.h"
#include "../Utils/Texture.h"

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

// Define static texture coordinates for the cube
const GLfloat Block::textureCoords[48] = {
        // Front face (vertices: 0, 1, 2, 3) - Flipped by 90 degrees
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
        // Back face (vertices: 4, 5, 6, 7) - Flipped by 90 degrees
        1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,
        // Bottom face (vertices: 0, 1, 5, 4)
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Top face (vertices: 2, 3, 7, 6)
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Left face (vertices: 0, 3, 7, 4)
        0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        // Right face (vertices: 1, 2, 6, 5)
        0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f
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
void Block::render(const World& world) const {
    if (!m_isVisible) return; // Only render if the block is visible

    // Enable depth testing and texture
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Check water boolean
    bool isWater = false;

    // Bind the texture based on block type
    switch (m_type) {
        case BlockType::DIRT:
            glBindTexture(GL_TEXTURE_2D, Texture::dirt.getNativeHandle());
            break;
        case BlockType::GRASS:
            glBindTexture(GL_TEXTURE_2D, Texture::grass.getNativeHandle());
            break;
        case BlockType::STONE:
            glBindTexture(GL_TEXTURE_2D, Texture::stone.getNativeHandle());
            break;
        case BlockType::WATER:
            glBindTexture(GL_TEXTURE_2D, Texture::water.getNativeHandle());
            isWater = true;
            break;
        case BlockType::PLANKS:
            glBindTexture(GL_TEXTURE_2D, Texture::planks.getNativeHandle());
            break;
        default:
            glBindTexture(GL_TEXTURE_2D, Texture::none.getNativeHandle());
            break;
    }

    // If rendering water, enable blending for transparency
    if (isWater) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    // Save the current matrix state
    glPushMatrix();

    // Translate to the block's position
    glTranslatef(static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_position.z));

    // Render the cube using triangles
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 36; i++) {
        int vertexIndex = indices[i];

        // Calculate face and triangle indices
        int faceIndex = i / 6;       // Determines which face we're rendering (6 vertices per face)
        int triangleIndex = i % 6;   // Determines which triangle of the face (0-2 for first triangle, 3-5 for second)

        // Texture coordinate assignment logic
        // Each face has four texture coordinates (4 vertices)
        int texCoordOffset = faceIndex * 8;

        // First triangle: Use the first three texture coordinates
        if (triangleIndex < 3) {
            // First triangle of the face (uses texture coords for vertices 0, 1, 2)
            glTexCoord2f(textureCoords[texCoordOffset + triangleIndex * 2], textureCoords[texCoordOffset + triangleIndex * 2 + 1]);
        }
            // Second triangle: Use the correct texture coordinates for vertices 2, 3, 0
        else {
            // Second triangle of the face (uses texture coords for vertices 2, 3, 0)
            int correctedIndex = (triangleIndex == 3) ? 2 : (triangleIndex == 4) ? 3 : 0;
            glTexCoord2f(textureCoords[texCoordOffset + correctedIndex * 2], textureCoords[texCoordOffset + correctedIndex * 2 + 1]);
        }


        // Set semi-transparent alpha for water, full opacity otherwise
        if (isWater) {
            glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // Water with 50% transparency
        }

        // Apply the vertex positions
        glVertex3f(vertices[3 * vertexIndex], vertices[3 * vertexIndex + 1], vertices[3 * vertexIndex + 2]);
    }
    glEnd();

    // Restore the previous matrix state
    glPopMatrix();

    // Disable blending if it was enabled
    if (isWater) {
        glDisable(GL_BLEND);
    }

    // Disable depth testing and texture
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}
