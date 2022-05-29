cmake_minimum_required(VERSION 3.20)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)

add_executable(RubiksCube main.cpp
        Structure/Cube.cpp Structure/Cube.h
        Object/Cube.cpp Object/Cube.h
        Object/DrawableInterface.cpp Object/DrawableInterface.h
        Object/CubeInterface.cpp Object/CubeInterface.h)

find_package(GLUT REQUIRED)
target_link_libraries(RubiksCube PRIVATE GLUT::GLUT)