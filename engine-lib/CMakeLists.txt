project(engine-lib)

file(GLOB_RECURSE ENGINE_PUBLIC_HEADERS include/graphics-engine/*.h)
file(GLOB_RECURSE ENGINE_SOURCES src/*.cc src/*.h)
add_library(engine-lib ${LIB_TYPE} ${ENGINE_PUBLIC_HEADERS} ${ENGINE_SOURCES})

if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
	target_compile_definitions(engine-lib PRIVATE BUILD_SHARED_LIBS)
else()
    set(LIB_TYPE STATIC)
endif()

target_link_libraries(engine-lib PRIVATE glad glm)
target_include_directories(engine-lib PRIVATE ${CMAKE_SOURCE_DIR}/third-patry/glad/include)

source_group("Interface Files" FILES ${ENGINE_PUBLIC_HEADERS})
source_group("Source Files" FILES ${ENGINE_SOURCES})

target_compile_options(engine-lib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wpedantic>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
)

target_include_directories(engine-lib PUBLIC include PRIVATE src)

if (BUILD_SHARED_LIBS)
  target_compile_definitions(engine-lib PRIVATE ENGINE_LIB_EXPORTS)
endif()

if(ENABLE_COVERAGE_GCC)
    message(STATUS "Coverage enabled for GCC!")
    target_compile_options(engine-lib PRIVATE -fprofile-arcs -ftest-coverage -g)
    target_link_options(engine-lib PRIVATE -fprofile-arcs -ftest-coverage)
endif()
