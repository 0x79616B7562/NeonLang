cmake_minimum_required(VERSION 3.28.2)

project(neon VERSION 0.1.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

add_definitions(-w)

file(
    GLOB
    SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*/*.cpp
)

add_executable(neon ${SRC_FILES})