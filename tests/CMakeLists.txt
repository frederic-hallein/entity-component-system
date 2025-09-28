add_executable(ecs_tests
    test_main.cpp
    test_world.cpp
    test_entityManager.cpp
    test_archetypeManager.cpp
    test_systems.cpp
)

target_precompile_headers(ecs_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include/pch.hpp
)

target_include_directories(ecs_tests PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_SOURCE_DIR}/libs/glad
    ${CMAKE_SOURCE_DIR}/libs/yaml-cpp/include
    ${CMAKE_SOURCE_DIR}/libs/imgui
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

target_link_libraries(ecs_tests
    gtest
    gtest_main
    pthread
    glad
    glfw
    OpenGL::GL
    yaml-cpp
    imgui
)

set_target_properties(ecs_tests PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

include(GoogleTest)
gtest_discover_tests(ecs_tests)