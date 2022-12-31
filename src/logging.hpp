#ifndef LOGGING_HPP
#define LOGGING_HPP

#include "spdlog/spdlog.h"

void initialize_logging();

#define LOG_DEBUG(text, ...) spdlog::debug((text), ##__VA_ARGS__);
#define LOG_INFO(text, ...) spdlog::info((text), ##__VA_ARGS__);
#define LOG_WARNING(text, ...) spdlog::warn((text), ##__VA_ARGS__);
#define LOG_ERROR(text, ...) spdlog::error((text), ##__VA_ARGS__);
#define LOG_CRITICAL(text, ...) spdlog::critical((text), ##__VA_ARGS__);

#endif
