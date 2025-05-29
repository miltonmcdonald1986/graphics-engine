add_executable(demo-app hello-world.cc)

target_link_libraries(demo-app PRIVATE engine-lib)
target_include_directories(demo-app PRIVATE ${CMAKE_SOURCE_DIR}/engine-lib/include)

set_target_properties(demo-app PROPERTIES
  VS_DEBUGGER_ENVIRONMENT "PATH=${CMAKE_BINARY_DIR}/engine-lib/$<CONFIG>;%PATH%"
)
