#include <iostream>
// #include <GLFW/glfw3.h>

#include "logger.hpp"

int main() {
    std::cout << "C++ Standard: " << __cplusplus << std::endl;

    LOG_INFO("Simple message");
    LOG_WARN("Warning with value: ", 42);
    LOG_ERROR("Error occurred with code: ", 500, " and message: ", "Internal server error");

    return 0;
}