cmake_minimum_required(VERSION 3.20.0)
project(imgui-serial)

# By default build a debug build.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configure Conan package manager.
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Export compile_commands.json file.
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Compile on Windows with unicode support.
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

# Locate clang-tidy
find_program(CLANGTIDY clang-tidy)
if(CLANGTIDY)
    message(STATUS "clang-tidy finished setting up.")
else()
    message(SEND_ERROR "Clang-Tidy requested but executable not found.")
endif()

# Setup source locations.
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(IMGUI_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui)
set(IMGUI_FILE_DIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/ImGuiFileDialog)

# Set system specific stuff.
execute_process(COMMAND uname OUTPUT_VARIABLE uname)
if (uname MATCHES "^MSYS" OR uname MATCHES "^MINGW")
    set(MSYS2 TRUE)
    set(OS_HELPER_IMPL ${SOURCE_DIR}/platform/os_helper_windows.cpp)
    message(STATUS "This is MSYS")
else()
    set(MSYS2 FALSE)
    set(OS_HELPER_IMPL ${SOURCE_DIR}/platform/os_helper_linux.cpp)
    message(STATUS "This is not MSYS")
endif()

set(EXT_SOURCES
    ${IMGUI_SOURCES_DIR}/imgui.cpp
    ${IMGUI_SOURCES_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCES_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCES_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCES_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.cpp
)
set(EXT_HEADERS
    ${IMGUI_SOURCES_DIR}/imgui.h
    ${IMGUI_SOURCES_DIR}/imconfig.h
    ${IMGUI_SOURCES_DIR}/imgui_internal.h
    ${IMGUI_SOURCES_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCES_DIR}/imstb_textedit.h
    ${IMGUI_SOURCES_DIR}/imstb_truetype.h
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl.h
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.h
    ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.h
)
set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/serial.cpp
    ${SOURCE_DIR}/logging.cpp
    ${SOURCE_DIR}/imgui_backend.cpp
    ${OS_HELPER_IMPL}
)
set(HEADERS
    ${SOURCE_DIR}/serial.hpp
    ${SOURCE_DIR}/logging.hpp
    ${SOURCE_DIR}/imgui_backend.hpp
    ${SOURCE_DIR}/file_dialog_config.hpp
)

# Find external packages.
find_package(OpenGL REQUIRED)
set(Boost_USE_STATIC_LIBS TRUE)
find_package(Boost REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Compile external stuff as a static library.
add_library(external STATIC ${EXT_HEADERS} ${EXT_SOURCES})
target_include_directories(
    external
    PUBLIC ${IMGUI_SOURCES_DIR}
    PUBLIC ${SDL2_INCLUDE_DIRS}
    PUBLIC ${IMGUI_SOURCES_DIR}/backends
    PUBLIC ${IMGUI_FILE_DIALOG_DIR}
)
target_link_libraries(external ${SDL2_LIBRARIES} ${Boost_LIBRARIES} ${CONAN_LIBS} OpenGL::GL)

# Add the main executable.
set(EXECUTABLE spc_player)
add_executable(${EXECUTABLE} WIN32)
target_sources(${EXECUTABLE} PRIVATE ${HEADERS} ${SOURCES})
target_link_libraries(${EXECUTABLE} external )
target_include_directories(
    ${EXECUTABLE}
    PRIVATE ${SOURCE_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

# Only enable clang-tidy checks for this target and related files.
set_target_properties(${EXECUTABLE} PROPERTIES CXX_CLANG_TIDY ${CLANGTIDY})

# Enable compiler warnings.
target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -Wformat -Wconversion -Wpedantic -Werror)

# On Windows MSYS2 link libraries statically to the binary. This way binary can
# be more easily distributed and does not depend on MSY2 being installed. On
# Linux just load used shared libraries instead.
if (MSYS2)
    target_link_libraries(${EXECUTABLE} ${SDL2_STATIC_LIBRARIES} ws2_32 -static)
else()
    target_link_libraries(${EXECUTABLE} ${SDL2_LIBRARIES})
endif()

target_compile_options(${EXECUTABLE} PRIVATE ${SDL2_CFLAGS_OTHER})

# Download Google test and set to use new policy.
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing and add test folder.
enable_testing()
add_subdirectory(test)
