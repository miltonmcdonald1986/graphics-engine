cmake_minimum_required(VERSION 3.15)
project(stb)

add_library(stb STATIC src/stb_image.cc src/stb_image_write.cc)

set_property(TARGET stb PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(stb PUBLIC include)
