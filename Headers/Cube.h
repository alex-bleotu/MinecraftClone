#ifndef MINECRAFTCLONE_CUBE_H
#define MINECRAFTCLONE_CUBE_H

#include <SFML/Graphics.hpp>
#include <array>

class Cube {
private:
    float size;

    std::vector<sf::Color> colors;

    sf::Vector2f position;
    sf::Vector3f rotation;

    std::vector<sf::Vector3f> vertices;
    std::vector<std::array<int, 4>> faces;

    void applyRotation(std::vector<sf::Vector3f>& transformedVertices, float angleX, float angleY, float angleZ) const;
    [[nodiscard]] sf::Vector2f projectPoint(const sf::Vector3f& point) const;
    void renderFace(sf::RenderWindow &window, const std::vector<sf::Vector2f>& projectedVertices, const std::array<int, 4>& face, sf::Color color) const;
    [[nodiscard]] float computeFaceDepth(const std::array<int, 4>& face, const std::vector<sf::Vector3f>& transformedVertices) const;
    void projectVertices(const std::vector<sf::Vector3f>& transformedVertices, std::vector<sf::Vector2f>& projectedVertices) const;

public:
    explicit Cube(float size);

    void render(sf::RenderWindow& window) const;
    void update(float& deltaTime);
    void rotate(float angleX, float angleY, float angleZ);

    void setSize(float size);
    void setPosition(sf::Vector2f position);
    void setVertices(std::vector<sf::Vector3f> vertices);
    void setRotation(sf::Vector3f rotation);
    void setColors(std::vector<sf::Color> colors);
};


#endif
