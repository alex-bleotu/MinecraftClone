# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.28)

# Declare the project name
project(MinecraftClone)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define the paths to the GLFW, GLAD, and SFML directories
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glfw/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/glfw/lib")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")
set(SFML_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/sfml/include")
set(SFML_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/sfml/lib")

# Include GLFW, GLAD, and SFML directories
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR} ${SFML_INCLUDE_DIR})

# Add GLAD as a static library
add_library(glad STATIC ${GLAD_SRC})

# Add GLFW as a static library
add_library(glfw STATIC IMPORTED)

# Specify the location of the GLFW static library for MinGW
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB_DIR}/libglfw3.a"
)

# Add SFML static libraries (make sure paths are correct)
find_library(SFML_SYSTEM sfml-system-s PATHS ${SFML_LIB_DIR})
find_library(SFML_WINDOW sfml-window-s PATHS ${SFML_LIB_DIR})
find_library(SFML_GRAPHICS sfml-graphics-s PATHS ${SFML_LIB_DIR})
find_library(SFML_AUDIO sfml-audio-s PATHS ${SFML_LIB_DIR})
find_library(SFML_NETWORK sfml-network-s PATHS ${SFML_LIB_DIR})
find_library(FREETYPE freetype PATHS ${SFML_LIB_DIR})
find_library(OPENAL openal32 PATHS ${SFML_LIB_DIR})

# Add the executable for your project
add_executable(MinecraftClone main.cpp
        Game.h
        Game.cpp
        src/UI/Scene.h
        src/UI/Scene.cpp
        src/UI/UserInterface.h
        src/UI/UserInterface.cpp
        src/Config.h
        src/Config.cpp
        src/Core/Block.h
        src/Core/Block.cpp
        src/Utils/Math.h
        src/Utils/Math.cpp
        src/Core/World.h
        src/Core/World.cpp
        src/Player/Player.h
        src/Player/Player.cpp
        src/Utils/Texture.h
        src/Utils/Texture.cpp
        src/Core/Chunk.h
        src/Utils/PerlinNoise.h
        src/Utils/PerlinNoise.cpp
        src/Core/Chunk.cpp)

# Link the necessary libraries (GLAD, GLFW, OpenGL, and SFML)
find_package(OpenGL REQUIRED)

# Link OpenGL, GLAD, GLFW, and SFML
target_link_libraries(MinecraftClone glad glfw OpenGL::GL
        ${SFML_SYSTEM} ${SFML_WINDOW} ${SFML_GRAPHICS} ${SFML_AUDIO} ${SFML_NETWORK}
        ${FREETYPE} ${OPENAL}
)

# Additional libraries for Windows
if (WIN32)
    target_link_libraries(MinecraftClone gdi32 user32 shell32)
endif()
