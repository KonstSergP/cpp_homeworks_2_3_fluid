cmake_minimum_required(VERSION 3.26)

project(cpp_homework_fluid)

set(CMAKE_CXX_STANDARD 20)

add_executable(main fluid.cpp
        headers/constants.h
        headers/fixed.h
        src/constants.cpp
        headers/Simulator.h
        headers/VectorField.h)