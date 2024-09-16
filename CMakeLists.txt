cmake_minimum_required(VERSION 3.28)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

# Add your executable and source files
add_executable(MinecraftClone main.cpp)

# Define the paths to the GLFW and GLAD directories
set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glfw/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/glfw/lib")
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/lib/glad/src/glad.c")

# Include GLFW and GLAD directories
include_directories(${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

# Add GLAD as a static library
add_library(glad STATIC ${GLAD_SRC})

# Add GLFW as a library
add_library(glfw STATIC IMPORTED)

# Specify the location of the GLFW library
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3.lib"  # For static linking on Windows
        # IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3.dll"  # For dynamic linking on Windows
)

# Link the necessary libraries (GLAD, GLFW, and OpenGL)
find_package(OpenGL REQUIRED)
target_link_libraries(MinecraftClone glad glfw OpenGL::GL)

# Additional libraries for Windows
if (WIN32)
    target_link_libraries(MinecraftClone opengl32)
endif()
