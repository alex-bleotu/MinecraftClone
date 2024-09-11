#include <cmath>
#include "Math.h"

Math::AABB::AABB(const sf::Vector3f &min, const sf::Vector3f &max) : min(min), max(max) {}

bool Math::AABB::intersects(const AABB &other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}
