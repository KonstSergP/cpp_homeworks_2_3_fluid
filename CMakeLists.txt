cmake_minimum_required(VERSION 3.26)

project(cpp_homework_fluid)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS)

add_executable(main fluid.cpp
        headers/constants.h
        headers/FixedImpl.h
        src/constants.cpp
        headers/Simulator.h
        headers/VectorField.h
        headers/Fixed.h)