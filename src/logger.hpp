#ifndef LOGGER_HPP
#define LOGGER_HPP

#ifdef _DEBUG
    #include <iostream>
    #include <sstream>
    #include <chrono>
    #include <iomanip>

    template<typename... Args>
    void debug_log(
        const std::string& level,
        const char* file,
        int line,
        Args&&... args)
    {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
                  << "[" << level << "] " << file << ":" << line << " - ";
        ((std::cout << args), ...);
        std::cout << std::endl;
    }

    #define LOG_INFO(...)  debug_log("INFO ", __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_WARN(...)  debug_log("WARN ", __FILE__, __LINE__, __VA_ARGS__)
    #define LOG_ERROR(...) debug_log("ERROR", __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
#endif

#endif