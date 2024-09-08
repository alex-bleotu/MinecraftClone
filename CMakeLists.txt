cmake_minimum_required(VERSION 3.28)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinecraftClone main.cpp
        Headers/Game.h
        Game.cpp
        Headers/Scene.h
        Scene.cpp
        Headers/UserInterface.h
        UserInterface.cpp
        Headers/Config.h
        Config.cpp
        Headers/Cube.h
        Cube.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/Program Files/SFML-2.6.1/include/SFML")
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories("C:/Program Files/SFML-2.6.1/include/SFML")
target_link_libraries(MinecraftClone sfml-system sfml-window sfml-graphics sfml-audio)
