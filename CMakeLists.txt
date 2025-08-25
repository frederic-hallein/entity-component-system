cmake_minimum_required(VERSION 3.28.3)
project(ecs VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.hpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
endif()

add_subdirectory(libs/glad)

# Configure GLFW build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)  # Disable Wayland
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)       # Enable X11

add_subdirectory(libs/glfw)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    libs/glfw/include
    libs/glad
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    OpenGL::GL
    glad
)

# Testing configuration
option(BUILD_TESTS "Build the tests" ON)

if(BUILD_TESTS)
    enable_testing()

    # Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest subdirectory
    add_subdirectory(libs/googletest)

    # Add tests subdirectory
    add_subdirectory(tests)
endif()