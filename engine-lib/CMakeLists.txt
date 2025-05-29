project(engine-lib)

file(GLOB_RECURSE ENGINE_PUBLIC_HEADERS include/graphics-engine/*.h)
file(GLOB_RECURSE ENGINE_SOURCES src/*.cc src/*.h)
add_library(engine-lib ${LIB_TYPE} ${ENGINE_PUBLIC_HEADERS} ${ENGINE_SOURCES})

target_include_directories(engine-lib PUBLIC include PRIVATE src)

set(LIB_TYPE_STATIC)
if (BUILD_SHARED_LIBS)
  set(LIB_TYPE_SHARED)
  target_compile_definitions(engine-lib PRIVATE ENGINE_LIB_EXPORTS)
endif()
