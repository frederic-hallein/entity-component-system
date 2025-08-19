#ifndef LOGGER_HPP
#define LOGGER_HPP

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"
#define COLOR_BLUE    "\033[34m"

template<typename... Args>
void debugLog(
    const std::string& level,
    const char* color,
    const char* file,
    int line,
    Args&&... args
)
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] "
                << color  << level << COLOR_RESET << " "
                << file << ":" << line << " - ";
    ((std::cout << args), ...);
    std::cout << std::endl;
}

#ifdef _DEBUG
    #define LOG_DEBUG(...) debugLog("[DEBUG]", COLOR_BLUE, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)  debugLog(" [INFO]", COLOR_GREEN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  debugLog(" [WARN]", COLOR_YELLOW, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) debugLog("[ERROR]", COLOR_RED, __FILE__, __LINE__, __VA_ARGS__)

#endif