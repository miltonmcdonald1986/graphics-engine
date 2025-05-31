file(GLOB_RECURSE SRC_FILES "src/*.h" "src/*.cc")
add_executable(demo-app ${SRC_FILES})

target_compile_options(demo-app PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wextra>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wpedantic>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
)

target_link_libraries(demo-app PRIVATE engine-lib)
target_include_directories(demo-app PRIVATE ${CMAKE_SOURCE_DIR}/engine-lib/include)

set_target_properties(demo-app PROPERTIES
  VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/engine-lib/$<CONFIG>;%PATH%"
)
