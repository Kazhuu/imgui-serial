#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstddef>

// General application constants.
const char* const APP_NAME = "SPC Player";
const char* const LOG_FILE = "logs/spc_player.txt";
const char* const LOGGER_NAME = "spc_player";
const int BAUD_RATE = 38400;
const int RESOLUTION_WIDTH = 1280;
const int RESOLUTION_HEIGHT = 720;
const int PORT_COUNT = 99;
const int LOG_FILE_SIZE_BYTES = 1024 * 1024 * 5;
const int LOG_FILE_COUNT = 3;
const int RESET_TIMEOUT_MS = 100;

// ImGui constants.
const float COMPONENT_WIDTH = 0.10F;

// Serial buffer sizes.
const size_t READ_BUFFER_SIZE = 500;
const size_t READ_QUEUE_SIZE = 1000;

// SDL2 OpenGL attributes.
const int ENABLE_DOUBLE_BUFFER = 1;
const int DEPTH_BUFFER_SIZE = 24;
const int STENCIL_BUFFER_SIZE = 8;

#endif
