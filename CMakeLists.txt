cmake_minimum_required(VERSION 3.26)

project(cpp_homework_fluid)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS -O4)
ADD_COMPILE_OPTIONS("-DTYPES=DOUBLE,FIXED(32, 16),FAST_FIXED(48,16)")
ADD_COMPILE_OPTIONS("-DSIZES=S(24,84),S(50,50)")
#ADD_COMPILE_OPTIONS("-fconstexpr-ops-limit=100000000")


add_executable(main main.cpp
        headers/constants.h
        headers/FixedImpl.h
        headers/Simulator.h
        headers/VectorField.h
        headers/Fixed.h
        headers/FieldInfo.h
        src/FieldInfo.cpp
        headers/CustomMatrix.h
        headers/parseSettings.h
        src/parseSettings.cpp
        headers/TypeGeneration.h
)