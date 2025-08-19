cmake_minimum_required(VERSION 3.28.3)
project(ecs VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/MyLib)
# target_link_libraries(${PROJECT_NAME} PUBLIC MyLib)

add_executable(${PROJECT_NAME} src/main.cpp)