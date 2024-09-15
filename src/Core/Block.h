#ifndef MINECRAFTCLONE_BLOCK_H
#define MINECRAFTCLONE_BLOCK_H


#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <array>
#include "../Utils/Math.h"

class World;

// Enum to define different block types
enum class BlockType {
    DIRT,
    GRASS,
    STONE,
    WATER,
    PLANKS,
    LOG,
    COBBLESTONE,
    LEAVES,
    CRAFTING_TABLE,
    FURNACE,
    IRON_ORE,
};

class Block {
public:
    // Default constructor
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
    bool checkIfVisible() const;

    // Check if block is opaque
    bool checkIfOpaque() const;

    // Check if block is solid
    bool checkIfSolid() const;

    // Render the block (for simplicity, could use SFML's RectangleShape or custom logic)
    void render() const;

    // Render the blocks that are not opaque
    void renderNotOpaque() const;

    // Get the bounding box of the block
    [[nodiscard]] Math::AABB getAABB() const;

private:
    BlockType type;                       // Type of block
    sf::Vector3i position;                // Position in the 3D world
    bool isVisible;                       // Whether the block is visible or not (AIR blocks are invisible)
    bool isOpaque;                        // Whether the block is opaque or not
    bool isSolid;                         // Whether the block is solid or not

    std::vector<sf::IntRect> textures;    // Textures for the block faces
    std::vector<int> textureRotation;     // Texture rotation for each face

    // Cube vertices
    static const GLfloat vertices[24];

    // Cube indices for drawing faces
    static const GLuint indices[36];
};


#endif
