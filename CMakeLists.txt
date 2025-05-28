cmake_minimum_required(VERSION 3.10)
project(graphics-engine VERSION 0.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(EXISTS "${CMAKE_SOURCE_DIR}/third-party/glfw/CMakeLists.txt")
    message(STATUS "GLFW submodule found! Adding to build.")
    add_subdirectory(third-party/glfw)
else()
    message(WARNING "GLFW submodule is missing! Run: git submodule update --init --recursive")
endif()

add_subdirectory(demo-app)
