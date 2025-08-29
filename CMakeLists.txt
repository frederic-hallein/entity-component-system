cmake_minimum_required(VERSION 3.28.3)
project(ecs VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

# Source files
file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.hpp)

# Debug definition
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE _DEBUG)
endif()

# Third-party libraries
add_subdirectory(libs/glad)
add_subdirectory(libs/glfw)
add_subdirectory(libs/yaml-cpp)

# ImGui (manual source list, no add_subdirectory)
set(IMGUI_SOURCES
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC libs/imgui libs/imgui/backends)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

# Main target includes and links
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/yaml-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    glfw
    OpenGL::GL
    glad
    yaml-cpp
)

# GLFW build options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)

# Testing configuration
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(libs/googletest)
    add_subdirectory(tests)
endif()