project(glad)

add_library(glad STATIC src/glad.cpp)

# Include directories
target_include_directories(glad PUBLIC include)
