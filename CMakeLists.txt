cmake_minimum_required(VERSION 3.28)

project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

# Add executable
add_executable(MinecraftClone
        main.cpp
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
        src/Core/Chunk.cpp
)

# Find and include OpenGL
find_package(OpenGL REQUIRED)

# Set SFML for static linking
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/Program Files/SFML-2.6.1/include/SFML")
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

# Include directories for SFML
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories("C:/Program Files/SFML-2.6.1/include/SFML")

# Link libraries: OpenGL, SFML, and system libraries
target_link_libraries(MinecraftClone
        OpenGL::GL     # OpenGL library
        sfml-system    # SFML core system module
        sfml-window    # SFML window module
        sfml-graphics  # SFML graphics module
        sfml-audio     # SFML audio module
        kernel32       # Windows core libraries
        advapi32
        ole32
        oleaut32
        uuid
        gdi32
        user32
        shell32
        winmm
        ws2_32
)
