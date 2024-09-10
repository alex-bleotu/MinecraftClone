#include <utility>
#include <valarray>

#include "Headers/Cube.h"

Cube::Cube(float size) : size(size), position(200, 200) {
    float sizeHalved = size / 2;

    vertices = {
            sf::Vector3f(-sizeHalved, -sizeHalved, -sizeHalved), // Front face
            sf::Vector3f(sizeHalved, -sizeHalved, -sizeHalved),
            sf::Vector3f(sizeHalved, sizeHalved, -sizeHalved),
            sf::Vector3f(-sizeHalved, sizeHalved, -sizeHalved),
            sf::Vector3f(-sizeHalved, -sizeHalved, sizeHalved),  // Back face
            sf::Vector3f(sizeHalved, -sizeHalved, sizeHalved),
            sf::Vector3f(sizeHalved, sizeHalved, sizeHalved),
            sf::Vector3f(-sizeHalved, sizeHalved, sizeHalved),
    };

    faces = {
            {0, 1, 2, 3}, // Front face
            {4, 5, 6, 7}, // Back face
            {0, 1, 5, 4}, // Top face
            {3, 2, 6, 7}, // Bottom face
            {0, 3, 7, 4}, // Left face
            {1, 2, 6, 5}  // Right face
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
    std::vector<sf::Vector3f> transformedVertices(8);
    applyRotation(transformedVertices, rotation.x, rotation.y, rotation.z);

    // Sort faces using Painter's Algorithm based on the average depth (z-value) of the face
    std::vector<std::pair<int, float>> faceDepths;
    for (int i = 0; i < faces.size(); ++i) {
        float depth = computeFaceDepth(faces[i], transformedVertices); // Compute depth using 3D z-values
        faceDepths.push_back({i, depth});
    }

    // Sort faces by depth (farthest to nearest)
    std::sort(faceDepths.begin(), faceDepths.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
        return a.second > b.second; // Sort by z-depth in descending order
    });

    // Project and render faces in sorted order
    std::vector<sf::Vector2f> projectedVertices(8);
    projectVertices(transformedVertices, projectedVertices);

    for (const auto& pair : faceDepths) {
        int faceIndex = pair.first;
        renderFace(window, projectedVertices, faces[faceIndex], colors[faceIndex]);
    }
}

void Cube::renderFace(sf::RenderWindow &window, const std::vector<sf::Vector2f>& projectedVertices, const std::array<int, 4>& face, const sf::Color color) const {
    sf::ConvexShape shape;
    shape.setPointCount(4);

    for (int i = 0; i < 4; ++i) {
        shape.setPoint(i, projectedVertices[face[i]]);
    }

    shape.setFillColor(color);

    window.draw(shape);
}

void Cube::applyRotation(std::vector<sf::Vector3f>& transformedVertices, float angleX, float angleY, float angleZ) const {
    for (int i = 0; i < vertices.size(); ++i) {
        float x = vertices[i].x;
        float y = vertices[i].y;
        float z = vertices[i].z;

        // Apply rotation around X-axis
        float newY = y * std::cos(angleX) - z * std::sin(angleX);
        float newZ = y * std::sin(angleX) + z * std::cos(angleX);
        y = newY;
        z = newZ;

        // Apply rotation around Y-axis
        float newX = x * std::cos(angleY) + z * std::sin(angleY);
        z = -x * std::sin(angleY) + z * std::cos(angleY);
        x = newX;

        // Apply rotation around Z-axis
        newX = x * std::cos(angleZ) - y * std::sin(angleZ);
        newY = x * std::sin(angleZ) + y * std::cos(angleZ);
        x = newX;
        y = newY;

        transformedVertices[i] = sf::Vector3f(x, y, z);
    }
}

float Cube::computeFaceDepth(const std::array<int, 4>& face, const std::vector<sf::Vector3f>& transformedVertices) const {
    float depth = 0.0f;
    for (int i = 0; i < 4; ++i) {
        depth += transformedVertices[face[i]].z; // Use the z-value of the 3D vertex
    }
    return depth / 4.0f; // Return the average z-value (depth) of the face
}

void Cube::projectVertices(const std::vector<sf::Vector3f>& transformedVertices, std::vector<sf::Vector2f>& projectedVertices) const {
    for (int i = 0; i < transformedVertices.size(); ++i) {
        projectedVertices[i] = projectPoint(transformedVertices[i]);
    }
}

sf::Vector2f Cube::projectPoint(const sf::Vector3f& point) const {
    float distance = 400;
    float factor = distance / (distance + point.z); // Perspective projection factor
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