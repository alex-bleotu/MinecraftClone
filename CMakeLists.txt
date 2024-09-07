cmake_minimum_required(VERSION 3.28)
project(MinecraftClone)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinecraftClone main.cpp
        Game.h
        Game.cpp
        Scene.h
        Scene.cpp
        UserInterface.h
        UserInterface.cpp
        Config.h
        Config.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/Program Files/SFML-2.6.1/include/SFML")
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories("C:/Program Files/SFML-2.6.1/include/SFML")
target_link_libraries(MinecraftClone sfml-system sfml-window sfml-graphics sfml-audio)
