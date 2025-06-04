cmake_minimum_required(VERSION 3.15)
project(glad)

add_library(glad STATIC src/glad.cpp)

set_property(TARGET glad PROPERTY POSITION_INDEPENDENT_CODE ON)

# Include directories
target_include_directories(glad PUBLIC include)
