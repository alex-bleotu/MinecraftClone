#include <math.h>
#include "Math.h"

void Math::setPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
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


Math::AABB::AABB(const sf::Vector3f &min, const sf::Vector3f &max) : min(min), max(max) {}

bool Math::AABB::intersects(const AABB &other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}
