#include "Block.h"
#include "../Utils/Texture.h"

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
        // Front face (counter-clockwise when viewed from the front)
        0, 1, 2, 2, 3, 0,

        // Back face (counter-clockwise when viewed from the back)
        4, 7, 6, 6, 5, 4,

        // Bottom face (counter-clockwise when viewed from below)
        0, 4, 5, 5, 1, 0,

        // Top face (counter-clockwise when viewed from above)
        2, 6, 7, 7, 3, 2,

        // Left face (counter-clockwise when viewed from the left)
        0, 3, 7, 7, 4, 0,

        // Right face (counter-clockwise when viewed from the right)
        1, 5, 6, 6, 2, 1
};

Block::Block(): isVisible(false), isOpaque(false), isSolid(false) {}

// Constructor to initialize block with type and position
Block::Block(BlockType type, const sf::Vector3i& position)
        : type(type), position(position), isVisible(true) {
    if (type == BlockType::WATER || type == BlockType::LEAVES)
        isOpaque = false;
    else isOpaque = true;

    if (type != BlockType::WATER)
        isSolid = true;
    else isSolid = false;

    auto var = Texture::initTextures(type);
    textures = var.first;
    textureRotation = var.second;
}

// Getter for the block type
BlockType Block::getType() const {
    return type;
}

// Setter for the block type
void Block::setType(BlockType type) {
    type = type;
}

// Getter for the block position
sf::Vector3i Block::getPosition() const {
    return position;
}

// Setter for the block position
void Block::setPosition(const sf::Vector3i& position) {
    this->position = position;
}

// Check if block is visible (i.e., not air)
bool Block::checkIfVisible() const {
    return isVisible;
}

// Render the block (opaque blocks)
void Block::render() const {
    if (!isVisible || !isOpaque) return; // Only render if the block is visible and opaque

    // Save the current matrix state
    glPushMatrix();

    // Translate to the block's position
    glTranslatef(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z));

    // Get the size of the texture atlas
    const float atlasSize = static_cast<float>(Texture::atlas.getSize().x); // assuming width == height

    // Loop through each face (6 faces total)
    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
        // Get the texture coordinates for the current face
        const sf::IntRect& texRect = textures[faceIndex]; // Assuming textures contains texture coords for each face

        // Normalize the texture coordinates (between 0 and 1)
        float texLeft = static_cast<float>(texRect.left) / atlasSize;
        float texRight = static_cast<float>(texRect.left + texRect.width) / atlasSize;
        float texTop = static_cast<float>(texRect.top) / atlasSize;
        float texBottom = static_cast<float>(texRect.top + texRect.height) / atlasSize;

        // Apply rotation based on the rotation value for this face
        int rotation = textureRotation[faceIndex]; // Get the rotation angle for this face

        // Define a 2D array to represent the 4 corners of the texture coordinates
        float texCoords[4][2] = {
                {texLeft, texBottom},  // Bottom-left
                {texRight, texBottom}, // Bottom-right
                {texRight, texTop},    // Top-right
                {texLeft, texTop}      // Top-left
        };

        // Rotate texture coordinates by rotating the order in which the coordinates are used
        if (rotation == 90) {
            // Rotate 90 degrees (clockwise): Swap positions
            std::swap(texCoords[0], texCoords[1]);
            std::swap(texCoords[1], texCoords[2]);
            std::swap(texCoords[2], texCoords[3]);
        } else if (rotation == 180) {
            // Rotate 180 degrees: Flip both horizontally and vertically
            std::swap(texCoords[0], texCoords[2]);
            std::swap(texCoords[1], texCoords[3]);
        } else if (rotation == 270 || rotation == -90) {
            // Rotate 270 degrees (counterclockwise): Swap in the reverse order
            std::swap(texCoords[0], texCoords[3]);
            std::swap(texCoords[3], texCoords[2]);
            std::swap(texCoords[2], texCoords[1]);
        }

        // Begin rendering triangles for this face
        glBegin(GL_TRIANGLES);

        // First triangle of the face (using rotated texture coordinates)
        glTexCoord2f(texCoords[0][0], texCoords[0][1]); // Bottom-left
        glVertex3f(vertices[indices[faceIndex * 6 + 0] * 3 + 0], vertices[indices[faceIndex * 6 + 0] * 3 + 1], vertices[indices[faceIndex * 6 + 0] * 3 + 2]);

        glTexCoord2f(texCoords[1][0], texCoords[1][1]); // Bottom-right
        glVertex3f(vertices[indices[faceIndex * 6 + 1] * 3 + 0], vertices[indices[faceIndex * 6 + 1] * 3 + 1], vertices[indices[faceIndex * 6 + 1] * 3 + 2]);

        glTexCoord2f(texCoords[2][0], texCoords[2][1]); // Top-right
        glVertex3f(vertices[indices[faceIndex * 6 + 2] * 3 + 0], vertices[indices[faceIndex * 6 + 2] * 3 + 1], vertices[indices[faceIndex * 6 + 2] * 3 + 2]);

        // Second triangle of the face
        glTexCoord2f(texCoords[2][0], texCoords[2][1]); // Top-right
        glVertex3f(vertices[indices[faceIndex * 6 + 3] * 3 + 0], vertices[indices[faceIndex * 6 + 3] * 3 + 1], vertices[indices[faceIndex * 6 + 3] * 3 + 2]);

        glTexCoord2f(texCoords[3][0], texCoords[3][1]); // Top-left
        glVertex3f(vertices[indices[faceIndex * 6 + 4] * 3 + 0], vertices[indices[faceIndex * 6 + 4] * 3 + 1], vertices[indices[faceIndex * 6 + 4] * 3 + 2]);

        glTexCoord2f(texCoords[0][0], texCoords[0][1]); // Bottom-left
        glVertex3f(vertices[indices[faceIndex * 6 + 5] * 3 + 0], vertices[indices[faceIndex * 6 + 5] * 3 + 1], vertices[indices[faceIndex * 6 + 5] * 3 + 2]);

        glEnd();
    }

    // Restore the previous matrix state
    glPopMatrix();
}

// Render the block (non-opaque blocks)
void Block::renderNotOpaque() const {
    if (!isVisible || isOpaque) return; // Only render if the block is visible and not opaque

    glPushMatrix();
    glTranslatef(static_cast<float>(position.x), static_cast<float>(position.y), static_cast<float>(position.z));

    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Standard alpha blending

    // Enable depth testing but disable depth writing for transparency handling
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);  // Disable writing to the depth buffer to prevent transparency issues

    // Check if the block is of type WATER and apply a transparency effect
    if (type == BlockType::WATER) {
        // Apply transparency for water blocks (you can adjust the alpha value to your liking)
        glColor4f(1.0f, 1.0f, 1.0f, 0.65f); // Set RGBA color with alpha = 0.5 for semi-transparency
    } else {
        // Apply transparency for other non-opaque blocks
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Set RGBA color with alpha
    }

    // Get the size of the texture atlas
    const float atlasSize = static_cast<float>(Texture::atlas.getSize().x);

    // Loop through each face (6 faces total)
    for (int faceIndex = 0; faceIndex < 6; faceIndex++) {
        // Get the texture coordinates for the current face
        const sf::IntRect& texRect = textures[faceIndex]; // Assuming textures contains texture coords for each face

        // Normalize the texture coordinates (between 0 and 1)
        float texLeft = static_cast<float>(texRect.left) / atlasSize;
        float texRight = static_cast<float>(texRect.left + texRect.width) / atlasSize;
        float texTop = static_cast<float>(texRect.top) / atlasSize;
        float texBottom = static_cast<float>(texRect.top + texRect.height) / atlasSize;

        // Apply rotation based on the rotation value for this face
        int rotation = textureRotation[faceIndex]; // Get the rotation angle for this face

        // Define a 2D array to represent the 4 corners of the texture coordinates
        float texCoords[4][2] = {
                {texLeft, texBottom},  // Bottom-left
                {texRight, texBottom}, // Bottom-right
                {texRight, texTop},    // Top-right
                {texLeft, texTop}      // Top-left
        };

        // Rotate texture coordinates by rotating the order in which the coordinates are used
        if (rotation == 90) {
            // Rotate 90 degrees (clockwise): Swap positions
            std::swap(texCoords[0], texCoords[1]);
            std::swap(texCoords[1], texCoords[2]);
            std::swap(texCoords[2], texCoords[3]);
        } else if (rotation == 180) {
            // Rotate 180 degrees: Flip both horizontally and vertically
            std::swap(texCoords[0], texCoords[2]);
            std::swap(texCoords[1], texCoords[3]);
        } else if (rotation == 270 || rotation == -90) {
            // Rotate 270 degrees (counterclockwise): Swap in the reverse order
            std::swap(texCoords[0], texCoords[3]);
            std::swap(texCoords[3], texCoords[2]);
            std::swap(texCoords[2], texCoords[1]);
        }

        // Begin rendering triangles for this face
        glBegin(GL_TRIANGLES);

        // First triangle of the face (using rotated texture coordinates)
        glTexCoord2f(texCoords[0][0], texCoords[0][1]); // Bottom-left
        glVertex3f(vertices[indices[faceIndex * 6 + 0] * 3 + 0], vertices[indices[faceIndex * 6 + 0] * 3 + 1], vertices[indices[faceIndex * 6 + 0] * 3 + 2]);

        glTexCoord2f(texCoords[1][0], texCoords[1][1]); // Bottom-right
        glVertex3f(vertices[indices[faceIndex * 6 + 1] * 3 + 0], vertices[indices[faceIndex * 6 + 1] * 3 + 1], vertices[indices[faceIndex * 6 + 1] * 3 + 2]);

        glTexCoord2f(texCoords[2][0], texCoords[2][1]); // Top-right
        glVertex3f(vertices[indices[faceIndex * 6 + 2] * 3 + 0], vertices[indices[faceIndex * 6 + 2] * 3 + 1], vertices[indices[faceIndex * 6 + 2] * 3 + 2]);

        // Second triangle of the face
        glTexCoord2f(texCoords[2][0], texCoords[2][1]); // Top-right
        glVertex3f(vertices[indices[faceIndex * 6 + 3] * 3 + 0], vertices[indices[faceIndex * 6 + 3] * 3 + 1], vertices[indices[faceIndex * 6 + 3] * 3 + 2]);

        glTexCoord2f(texCoords[3][0], texCoords[3][1]); // Top-left
        glVertex3f(vertices[indices[faceIndex * 6 + 4] * 3 + 0], vertices[indices[faceIndex * 6 + 4] * 3 + 1], vertices[indices[faceIndex * 6 + 4] * 3 + 2]);

        glTexCoord2f(texCoords[0][0], texCoords[0][1]); // Bottom-left
        glVertex3f(vertices[indices[faceIndex * 6 + 5] * 3 + 0], vertices[indices[faceIndex * 6 + 5] * 3 + 1], vertices[indices[faceIndex * 6 + 5] * 3 + 2]);

        glEnd();
    }

    // Restore depth writing after rendering transparent block
    glDepthMask(GL_TRUE);  // Re-enable writing to the depth buffer
    glDisable(GL_BLEND);   // Disable blending

    glPopMatrix();         // Restore matrix state
}

// Get the bounding box of the block
Math::AABB Block::getAABB() const {
    sf::Vector3f minPos(position.x, position.y, position.z);
    sf::Vector3f maxPos = minPos + sf::Vector3f(1.0f, 1.0f, 1.0f);
    return {minPos, maxPos};
}

// Check if block is opaque
bool Block::checkIfOpaque() const {
    return isOpaque;
}

// Check if block is solid
bool Block::checkIfSolid() const {
    return isSolid;
}