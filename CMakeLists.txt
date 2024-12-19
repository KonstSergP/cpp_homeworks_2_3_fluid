cmake_minimum_required(VERSION 3.26)

project(cpp_homework_fluid)

set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_CXX_FLAGS -O4)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g3 -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O4")

include_directories("headers/")


add_executable(main main.cpp
        src/Fixed.cpp
        src/FieldInfo.cpp
        src/Simulator.cpp
        src/ThreadPool.cpp
        src/parseSettings.cpp
)
