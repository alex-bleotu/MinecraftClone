#include <cmath>
#include "Math.h"

Math::AABB::AABB(const sf::Vector3f &min, const sf::Vector3f &max) : min(min), max(max) {}

bool Math::AABB::intersects(const AABB &other) const {
    float epsilon = 0.001f;  // Small buffer to avoid floating-point precision issues

    return (max.x > other.min.x - epsilon && min.x < other.max.x + epsilon) &&
           (max.y > other.min.y - epsilon && min.y < other.max.y + epsilon) &&
           (max.z > other.min.z - epsilon && min.z < other.max.z + epsilon);
}
