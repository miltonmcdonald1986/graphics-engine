set(ENV{PATH} "${CMAKE_BINARY_DIR}/bin/$<CONFIG>;$ENV{PATH}")
message("Current PATH: $ENV{PATH}")

file(GLOB TEST_SOURCES "src/*.cc")

add_executable(engine-tests ${TEST_SOURCES})
add_test(NAME EngineTests COMMAND engine-tests)
target_include_directories(engine-tests PRIVATE third-party/googletest/googletest/include)
target_link_libraries(engine-tests PRIVATE engine-lib gtest gtest_main)

set_target_properties(engine-tests PROPERTIES
  VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/bin/$<CONFIG>;${CMAKE_BINARY_DIR}/engine-lib/$<CONFIG>;%PATH%"
)

if(ENABLE_COVERAGE_GCC)
    message(STATUS "Coverage enabled for gcc!")
    target_compile_options(engine-tests PRIVATE -fprofile-arcs -ftest-coverage -O0 -g)
    target_link_options(engine-tests PRIVATE -fprofile-arcs -ftest-coverage)
endif()
