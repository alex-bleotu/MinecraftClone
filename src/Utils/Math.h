#ifndef MINECRAFTCLONE_UTILS_H
#define MINECRAFTCLONE_UTILS_H


#include <SFML/OpenGL.hpp>
#include <System/Vector3.hpp>
#include <functional>

namespace Math {

    // Custom gluPerspective function
    void static setPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

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
