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

void Math::gluLookAt(float eyeX, float eyeY, float eyeZ,
                  float centerX, float centerY, float centerZ,
                  float upX, float upY, float upZ) {
    float forward[3], up[3], side[3], matrix[16];
    float fLength, uLength, sLength;

    // Calculate forward vector (center - eye)
    forward[0] = centerX - eyeX;
    forward[1] = centerY - eyeY;
    forward[2] = centerZ - eyeZ;

    // Normalize the forward vector
    fLength = std::sqrt(forward[0] * forward[0] + forward[1] * forward[1] + forward[2] * forward[2]);
    forward[0] /= fLength;
    forward[1] /= fLength;
    forward[2] /= fLength;

    // Copy the up vector
    up[0] = upX;
    up[1] = upY;
    up[2] = upZ;

    // Calculate the side vector (side = forward x up)
    side[0] = forward[1] * up[2] - forward[2] * up[1];
    side[1] = forward[2] * up[0] - forward[0] * up[2];
    side[2] = forward[0] * up[1] - forward[1] * up[0];

    // Normalize the side vector
    sLength = std::sqrt(side[0] * side[0] + side[1] * side[1] + side[2] * side[2]);
    side[0] /= sLength;
    side[1] /= sLength;
    side[2] /= sLength;

    // Recalculate up vector (up = side x forward)
    up[0] = side[1] * forward[2] - side[2] * forward[1];
    up[1] = side[2] * forward[0] - side[0] * forward[2];
    up[2] = side[0] * forward[1] - side[1] * forward[0];

    // Build the rotation matrix
    matrix[0] = side[0];
    matrix[1] = up[0];
    matrix[2] = -forward[0];
    matrix[3] = 0.0f;

    matrix[4] = side[1];
    matrix[5] = up[1];
    matrix[6] = -forward[1];
    matrix[7] = 0.0f;

    matrix[8] = side[2];
    matrix[9] = up[2];
    matrix[10] = -forward[2];
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;

    // Apply the transformation to the current matrix
    glMultMatrixf(matrix);

    // Apply the translation part
    glTranslatef(-eyeX, -eyeY, -eyeZ);
}
