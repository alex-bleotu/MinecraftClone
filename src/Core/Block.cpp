#include "Block.h"
#include "../Utils/Texture.h"

// Function to convert enum to string
static std::string blockTypeToString(BlockType type) {
    switch (type) {
        case BlockType::AIR:                return "air";
        case BlockType::DIRT:               return "dirt";
        case BlockType::GRASS:              return "grass";
        case BlockType::STONE:              return "stone";
        case BlockType::WATER:              return "water";
        case BlockType::PLANKS:             return "planks";
        default:                            return "unknown";
    }
}

// Define static vertices for the cube
const GLfloat Block::vertices[24] = {
        0.0f, 0.0f, 0.0f,  // Vertex 0 (Front bottom left)
        1.0f, 0.0f, 0.0f,  // Vertex 1 (Front bottom right)
        1.0f, 1.0f, 0.0f,  // Vertex 2 (Front top right)
        0.0f, 1.0f, 0.0f,  // Vertex 3 (Front top left)
        0.0f, 0.0f, 1.0f,  // Vertex 4 (Back bottom left)
        1.0f, 0.0f, 1.0f,  // Vertex 5 (Back bottom right)
        1.0f, 1.0f, 1.0f,  // Vertex 6 (Back top right)
        0.0f, 1.0f, 1.0f   // Vertex 7 (Back top left)
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
        // Front face (vertices: 0, 1, 2, 3) - Rotated 180 degrees
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Back face (vertices: 4, 5, 6, 7) - Rotated 180 degrees
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Bottom face (vertices: 0, 1, 5, 4) - No change
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Top face (vertices: 2, 3, 7, 6) - No change
        0.0f, 0.0f,  1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,
        // Left face (vertices: 0, 3, 7, 4) - Rotated 180 degrees
        1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f,
        // Right face (vertices: 1, 2, 6, 5) - Rotated 180 degrees
        1.0f, 0.0f,  1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f
};

Block::Block() : m_type(BlockType::AIR), m_position(0, 0, 0), m_isVisible(false), m_textures() {}


// Constructor to initialize block with type and position
Block::Block(BlockType type, const sf::Vector3i& position)
        : m_type(type), m_position(position) {
    // If the block is AIR, it's not visible
    m_isVisible = (type != BlockType::AIR);

    if (m_isVisible)
        m_textures = Texture::readTextures(blockTypeToString(m_type));
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

    // Enable depth testing and texture
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Save the current matrix state
    glPushMatrix();

    // Translate to the block's position
    glTranslatef(static_cast<float>(m_position.x), static_cast<float>(m_position.y), static_cast<float>(m_position.z));

    // Loop through each face
    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
        // Bind the appropriate texture for this face before glBegin
        if (faceIndex < m_textures.size() && m_textures[faceIndex].getNativeHandle() != 0) {
            glBindTexture(GL_TEXTURE_2D, m_textures[faceIndex].getNativeHandle());
        } else {
            glBindTexture(GL_TEXTURE_2D, Texture::none.getNativeHandle()); // Fallback if no texture is set for the face
        }

        // Begin rendering triangles for this face
        glBegin(GL_TRIANGLES);

        int texCoordOffset = faceIndex * 8; // Each face has four texture coordinates (4 vertices)

        // Render the two triangles for each face
        for (int triangleIndex = 0; triangleIndex < 6; triangleIndex++) {
            int vertexIndex = indices[faceIndex * 6 + triangleIndex];

            // Apply the correct texture coordinates
            int texCoordIndex = triangleIndex < 3 ? triangleIndex : (triangleIndex == 3) ? 2 : (triangleIndex == 4) ? 3 : 0;
            glTexCoord2f(textureCoords[texCoordOffset + texCoordIndex * 2], textureCoords[texCoordOffset + texCoordIndex * 2 + 1]);

            // Apply the vertex positions
            glVertex3f(vertices[3 * vertexIndex], vertices[3 * vertexIndex + 1], vertices[3 * vertexIndex + 2]);
        }

        glEnd();
    }

    // Restore the previous matrix state
    glPopMatrix();

    // Disable depth testing and texture
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
}

// Get the bounding box of the block
Math::AABB Block::getAABB() const {
    sf::Vector3f minPos(m_position.x, m_position.y, m_position.z);
    sf::Vector3f maxPos = minPos + sf::Vector3f(1.0f, 1.0f, 1.0f);
    return {minPos, maxPos};
}