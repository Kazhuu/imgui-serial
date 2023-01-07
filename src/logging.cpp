#include "logging.hpp"
#include "config.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <memory>

/**
 * Initialize file rotating logging and set it as a default application wide
 * logger.
 */
void initialize_logging() {
    spdlog::set_error_handler(
        [](const std::string& msg) { spdlog::get("console")->error("*** LOGGER ERROR ***: {}", msg); });

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LOG_FILE, LOG_FILE_SIZE_BYTES,
                                                                            LOG_FILE_COUNT);

    spdlog::sinks_init_list sink_list = {file_sink, console_sink};

    auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sink_list);
    logger->set_level(spdlog::level::debug);
    // Always flush error level and above immediately when logged.
    logger->flush_on(spdlog::level::err);
    spdlog::set_default_logger(logger);
}
