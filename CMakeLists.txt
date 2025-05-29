cmake_minimum_required(VERSION 3.10)
project(graphics-engine VERSION 0.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

if(EXISTS "${CMAKE_SOURCE_DIR}/third-party/glfw/CMakeLists.txt")
  message(STATUS "GLFW submodule found! Adding to build.")
  add_subdirectory(third-party/glfw)
else()
  message(WARNING "GLFW submodule is missing! Run: git submodule update --init --recursive")
endif()

add_subdirectory(engine-lib)
add_subdirectory(demo-app)

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT demo-app)
