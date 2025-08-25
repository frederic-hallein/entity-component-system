# Create test executable
add_executable(ecs_tests
    test_main.cpp
    test_component_manager.cpp
    # test_entity_manager.cpp
)

# Link against Google Test and your project dependencies
target_link_libraries(ecs_tests
    gtest
    gtest_main
    pthread
    glad
    glfw
    OpenGL::GL
)

# Include directories
target_include_directories(ecs_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/glad
    ${CMAKE_SOURCE_DIR}/libs/glfw/include
)

# Use precompiled headers
target_precompile_headers(ecs_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include/pch.hpp
)

# Set C++ standard to match main project
set_target_properties(ecs_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Discover tests for CTest
include(GoogleTest)
gtest_discover_tests(ecs_tests)