#ifndef LOGGER_HPP
#define LOGGER_HPP

// ANSI color codes
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_RED     "\033[31m"
#define COLOR_BLUE    "\033[34m"

template<typename... Args>
void logger(
    const str& level,
    cstr color,
    cstr file,
    i64 line,
    Args&&... args
)
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::cout << color  << level << COLOR_RESET << " "
              << file << ":" << line << " - ";
    ((std::cout << args), ...);
    std::cout << std::endl;
}

#ifdef _DEBUG
    #define LOG_DEBUG(...) logger("[DEBUG]", COLOR_BLUE, __FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)  logger(" [INFO]", COLOR_GREEN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  logger(" [WARN]", COLOR_YELLOW, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logger("[ERROR]", COLOR_RED, __FILE__, __LINE__, __VA_ARGS__)

#endif