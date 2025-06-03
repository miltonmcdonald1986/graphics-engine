project(demo-app)

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

target_link_libraries(demo-app PRIVATE engine-lib glfw)
target_include_directories(demo-app PRIVATE ${CMAKE_SOURCE_DIR}/engine-lib/include)

set_target_properties(demo-app PROPERTIES
  VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/engine-lib/$<CONFIG>;%PATH%"
)

if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET demo-app
        POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_BINARY_DIR}/engine-lib/$<CONFIG>
            ${CMAKE_BINARY_DIR}/demo-app/$<CONFIG>
		COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/third-party/glfw/src/$<CONFIG>/glfw3.dll
            ${CMAKE_BINARY_DIR}/demo-app/$<CONFIG>
    )
endif()