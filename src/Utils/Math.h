#ifndef MINECRAFTCLONE_UTILS_H
#define MINECRAFTCLONE_UTILS_H


#include <SFML/OpenGL.hpp>
#include <SFML/System/Vector3.hpp>
#include <functional>
#include <cmath>
#include <SFML/System/Vector2.hpp>

namespace Math {
    // Custom gluPerspective function
    void static setPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
        float top = nearPlane * tan(fov * M_PI / 360.0);
        float bottom = -top;
        float right = top * aspectRatio;
        float left = -right;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(left, right, bottom, top, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    // Axis-aligned bounding box (AABB) for collision detection
    struct AABB {
        sf::Vector3f min;
        sf::Vector3f max;

        AABB(const sf::Vector3f &min, const sf::Vector3f &max);

        AABB(float x, float y, float z, float width, float height, float depth);

        AABB(int x, int y, int z, int width, int height, int depth);

        [[nodiscard]] bool intersects(const AABB &other) const;
    };
}

namespace std {
    template<>
    struct hash<sf::Vector3i> {
        std::size_t operator()(const sf::Vector3i &v) const {
            // Combine the x, y, and z coordinates into a single hash value
            return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1) ^ (std::hash<int>()(v.z) << 1);
        }
    };

    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            std::size_t h1 = std::hash<int>()(v.x);
            std::size_t h2 = std::hash<int>()(v.y);
            return h1 ^ (h2 << 1);  // Combine the two hashes
        }
    };
}

#endif
