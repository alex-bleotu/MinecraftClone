#ifndef MINECRAFTCLONE_UTILS_H
#define MINECRAFTCLONE_UTILS_H


#include <SFML/OpenGL.hpp>

class Math {
public:
    // Custom gluPerspective function
    void static setPerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);

    // Custom lookAt function
    void static gluLookAt(float eyeX, float eyeY, float eyeZ,
                      float centerX, float centerY, float centerZ,
                      float upX, float upY, float upZ);
};


#endif
