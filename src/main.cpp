#include "pch.hpp"


int main() {
    printSystemInfo();

    LOG_INFO("Simple message");
    LOG_WARN("Warning with value: ", 42);
    LOG_ERROR("Error occurred with code: ", 500, " and message: ", "Internal server error");
    LOG_DEBUG("Debug: ", "something to debug");

    return 0;
}