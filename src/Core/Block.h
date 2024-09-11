#ifndef MINECRAFTCLONE_BLOCK_H
#define MINECRAFTCLONE_BLOCK_H


#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <array>
#include "../Utils/Math.h"

class World;

// Enum to define different block types
enum class BlockType {
    AIR,    // Empty block
    DIRT,   // Dirt block
    GRASS,  // Grass block
    STONE,  // Stone block
    WATER,  // Water block
    PLANKS,   // Wood block
    // Add more types as needed
};

class Block {
public:
    // Constructor to initialize block with type and position
    Block();

    // Constructor to initialize block with type and position
    Block(BlockType type, const sf::Vector3i& position);

    // Getter for the block type
    [[nodiscard]] BlockType getType() const;

    // Setter for the block type
    void setType(BlockType type);

    // Getter for the block position
    [[nodiscard]] sf::Vector3i getPosition() const;

    // Setter for the block position
    void setPosition(const sf::Vector3i& position);

    // Check if block is visible (i.e., not air)
    bool isVisible() const;

    // Render the block (for simplicity, could use SFML's RectangleShape or custom logic)
    void render(const World& world) const;

    // Get the bounding box of the block
    Math::AABB getBoundingBox() const;

private:
    BlockType m_type;            // Type of block (AIR, DIRT, etc.)
    sf::Vector3i m_position;     // Position in the 3D world
    bool m_isVisible;            // Whether the block is visible or not (AIR blocks are invisible)

    // Cube vertices
    static const GLfloat vertices[24];

    // Cube indices for drawing faces
    static const GLuint indices[36];

    // Cube texture coordinates
    static const GLfloat textureCoords[48];
};


#endif
