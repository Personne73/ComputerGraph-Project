cmake_minimum_required(VERSION 3.28)
project(ComputerGraphics)

set(CMAKE_CXX_STANDARD 17)

add_executable(ComputerGraphics main.cpp
        Intro/testgl.cpp
        TD1/GLShader.cpp
        TD1/GLShader.h
        TD1/exemple_opengl2.cpp)
