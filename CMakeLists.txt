project(xenmm)
cmake_minimum_required(VERSION 2.8)

option(ONLY_CLIENT Off)

add_subdirectory(script)
add_subdirectory(src)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(unit)
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
