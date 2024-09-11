#ifndef MINECRAFTCLONE_UTILS_H
#define MINECRAFTCLONE_UTILS_H


#include <SFML/OpenGL.hpp>
#include <System/Vector3.hpp>
#include <functional>
#include <cmath>

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
}

#endif
