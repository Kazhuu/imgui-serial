#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "spdlog/spdlog.h"

void initialize_logging();

template<typename... Args>
inline void log_debug(spdlog::format_string_t<Args...> fmt, Args &&... args) {
    spdlog::debug(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_info(spdlog::format_string_t<Args...> fmt, Args &&... args) {
    spdlog::info(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_warning(spdlog::format_string_t<Args...> fmt, Args &&... args) {
    spdlog::warn(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_error(spdlog::format_string_t<Args...> fmt, Args &&... args) {
    spdlog::error(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
inline void log_critical(spdlog::format_string_t<Args...> fmt, Args &&... args) {
    spdlog::critical(fmt, std::forward<Args>(args)...);
}

#endif
