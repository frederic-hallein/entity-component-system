#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP

#include "logger.hpp"
#include "common.hpp"

void printBuiltType(cstr builtType) {
    LOG_INFO("Built Type: ", builtType);
}

void printCppStandard() {
    i64 cppStandard = __cplusplus;
    if (cppStandard >= 202302L) {
        LOG_INFO("C++ Standard: C++23 (", cppStandard, ")");
    } else if (cppStandard >= 202002L) {
        LOG_INFO("C++ Standard: C++20 (", cppStandard, ")");
    } else if (cppStandard >= 201703L) {
        LOG_INFO("C++ Standard: C++17 (", cppStandard, ")");
    } else if (cppStandard >= 201402L) {
        LOG_INFO("C++ Standard: C++14 (", cppStandard, ")");
    } else if (cppStandard >= 201103L) {
        LOG_INFO("C++ Standard: C++11 (", cppStandard, ")");
    } else if (cppStandard >= 199711L) {
        LOG_INFO("C++ Standard: C++98/03 (", cppStandard, ")");
    } else {
        LOG_INFO("C++ Standard: Pre-standard (", cppStandard, ")");
    }
}

void printCompilerInfo() {
    #if defined(__clang__)
        LOG_INFO("Compiler: Clang ", __clang_major__, ".", __clang_minor__, ".", __clang_patchlevel__);
    #elif defined(__GNUC__)
        LOG_INFO("Compiler: GCC ", __GNUC__, ".", __GNUC_MINOR__, ".", __GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        LOG_INFO("Compiler: MSVC ", _MSC_VER);
    #else
        LOG_INFO("Compiler: Unknown");
    #endif
}

void printPlatformInfo() {
    #ifdef _WIN32
        #ifdef _WIN64
            LOG_INFO("Platform: Windows (64-bit)");
        #else
            LOG_INFO("Platform: Windows (32-bit)");
        #endif
    #elif defined(__linux__)
        LOG_INFO("Platform: Linux");
    #elif defined(__APPLE__)
        LOG_INFO("Platform: macOS");
    #elif defined(__unix__)
        LOG_INFO("Platform: Unix");
    #else
        LOG_INFO("Platform: Unknown");
    #endif
}

void printArchitectureInfo() {
    #if defined(__x86_64__) || defined(_M_X64)
        LOG_INFO("Architecture: x64");
    #elif defined(__i386__) || defined(_M_IX86)
        LOG_INFO("Architecture: x86");
    #elif defined(__aarch64__) || defined(_M_ARM64)
        LOG_INFO("Architecture: ARM64");
    #elif defined(__arm__) || defined(_M_ARM)
        LOG_INFO("Architecture: ARM");
    #else
        LOG_INFO("Architecture: Unknown");
    #endif
}

void printSystemInfo() {
    LOG_INFO("=== System Information ===");
    printCppStandard();
    printCompilerInfo();
    printPlatformInfo();
    printArchitectureInfo();
    LOG_INFO("===========================\n");
}

void printInfo() {
#ifdef _DEBUG
    printBuiltType("DEBUG\n");
    printSystemInfo();
#else
    printBuiltType("RELEASE\n");
#endif
}

#endif