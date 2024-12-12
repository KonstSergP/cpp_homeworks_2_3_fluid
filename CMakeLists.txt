cmake_minimum_required(VERSION 3.26)

project(cpp_homework_fluid)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS)
ADD_COMPILE_OPTIONS("-DTYPES=FLOAT,DOUBLE,FIXED(32, 16),FIXED(64, 32),FAST_FIXED(25, 11)")

add_executable(main main.cpp
        headers/constants.h
        headers/FixedImpl.h
        headers/Simulator.h
        headers/VectorField.h
        headers/Fixed.h
        headers/FieldInfo.h
        src/FieldInfo.cpp
        headers/CustomMatrix.h
)