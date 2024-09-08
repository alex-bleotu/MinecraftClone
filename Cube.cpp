#include <utility>
#include <valarray>
#include <algorithm>

#include "Headers/Cube.h"

Cube::Cube(float size) : size(size), position(200, 200) {
    vertices = {
            sf::Vector3f(-size / 2, -size / 2, -size / 2), // Front face
            sf::Vector3f(size / 2, -size / 2, -size / 2),
            sf::Vector3f(size / 2, size / 2, -size / 2),
            sf::Vector3f(-size / 2, size / 2, -size / 2),
            sf::Vector3f(-size / 2, -size / 2, size / 2),  // Back face
            sf::Vector3f(size / 2, -size / 2, size / 2),
            sf::Vector3f(size / 2, size / 2, size / 2),
            sf::Vector3f(-size / 2, size / 2, size / 2),
    };

    faces = {
            {0, 1, 2}, {0, 2, 3}, // Front face (2 triangles)
            {4, 5, 6}, {4, 6, 7}, // Back face (2 triangles)
            {0, 1, 5}, {0, 5, 4}, // Top face (2 triangles)
            {3, 2, 6}, {3, 6, 7}, // Bottom face (2 triangles)
            {0, 3, 7}, {0, 7, 4}, // Left face (2 triangles)
            {1, 2, 6}, {1, 6, 5}  // Right face (2 triangles)
    };

    colors = {
            sf::Color::Red,
            sf::Color::Green,
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color::Magenta,
            sf::Color::Cyan
    };

    rotation = {0, 0, 0};
}

void Cube::render(sf::RenderWindow &window) const {
    std::vector<sf::Vector2f> projectedVertices(8);
    applyRotationAndProjection(projectedVertices, rotation.x, rotation.y, rotation.z);

    std::vector<std::pair<float, std::array<int, 3>>> faceDepths;
    calculateFaceDepths(faceDepths);

    std::sort(faceDepths.begin(), faceDepths.end(),
              [](const std::pair<float, std::array<int, 3>>& a, const std::pair<float, std::array<int, 3>>& b) {
                  return a.first > b.first;
              });

    for (const auto& faceDepth : faceDepths) {
        const auto& face = faceDepth.second;
        renderFace(window, projectedVertices, face, colors[&face - &faces[0]]);
    }
}

void Cube::renderFace(sf::RenderWindow &window, const std::vector<sf::Vector2f>& projectedVertices, const std::array<int, 3>& face, const sf::Color color) const {
    sf::ConvexShape shape;
    shape.setPointCount(3);

    for (int i = 0; i < 3; ++i) {
        shape.setPoint(i, projectedVertices[face[i]]);
    }

    shape.setFillColor(color);

    window.draw(shape);
}

void Cube::applyRotationAndProjection(std::vector<sf::Vector2f>& projectedVertices, float angleX, float angleY, float angleZ) const {
    for (int i = 0; i < vertices.size(); ++i) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        float z = vertices[i].z;

        float newY = y * std::cos(angleX) - z * std::sin(angleX);
        float newZ = y * std::sin(angleX) + z * std::cos(angleX);
        y = newY;
        z = newZ;

        float newX = x * std::cos(angleY) + z * std::sin(angleY);
        z = -x * std::sin(angleY) + z * std::cos(angleY);
        x = newX;

        newX = x * std::cos(angleZ) - y * std::sin(angleZ);
        newY = x * std::sin(angleZ) + y * std::cos(angleZ);
        x = newX;
        y = newY;

        projectedVertices[i] = projectPoint(sf::Vector3f(x, y, z));
    }
}

void Cube::calculateFaceDepths(std::vector<std::pair<float, std::array<int, 3>>>& faceDepths) const {
    for (auto face : faces) {
        float avgDepth = (vertices[face[0]].z + vertices[face[1]].z + vertices[face[2]].z) / 3.0f;
        faceDepths.emplace_back(avgDepth, face);
    }
}

sf::Vector2f Cube::projectPoint(const sf::Vector3f& point) const {
    float distance = 400;
    float factor = distance / (distance + point.z);
    float x = point.x * factor + position.x;
    float y = point.y * factor + position.y;
    return {x, y};
}

void Cube::update(float &deltaTime) {}

void Cube::rotate(float angleX, float angleY, float angleZ) {
    rotation.x += angleX;
    rotation.y += angleY;
    rotation.z += angleZ;
}

void Cube::setSize(float size) {
    this->size = size;
}

void Cube::setPosition(sf::Vector2f position) {
    this->position = position;
}

void Cube::setVertices(std::vector<sf::Vector3f> vertices) {
    this->vertices = std::move(vertices);
}

void Cube::setRotation(sf::Vector3f rotation) {
    this->rotation = rotation;
}

void Cube::setColors(std::vector<sf::Color> colors) {
    this->colors = std::move(colors);
}
