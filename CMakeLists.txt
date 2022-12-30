cmake_minimum_required(VERSION 3.20.0)
project(imgui-serial)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Export compile_commands.json file.
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Compile on Windows with unicode support.
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

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

set(IMGUI_SOURCES
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
set(SOURCES
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/serial.cpp
    ${OS_HELPER_IMPL}
)
set(HEADERS
    ${SOURCE_DIR}/serial.hpp
    ${SOURCE_DIR}/file_dialog_config.hpp
    # External headers.
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

# Find external packages.
find_package(OpenGL REQUIRED)

set(Boost_USE_STATIC_LIBS TRUE)
find_package(Boost REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# Executable
set(EXECUTABLE imgui_example)
add_executable(${EXECUTABLE} WIN32)
target_sources(${EXECUTABLE} PUBLIC ${HEADERS} PRIVATE ${IMGUI_SOURCES} ${SOURCES})

# Enable compiler warnings.
target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wformat -Wpedantic -Werror -municode)

target_include_directories(
    ${EXECUTABLE}
    PRIVATE ${SOURCE_DIR}
    PRIVATE ${SDL2_INCLUDE_DIRS}
    PRIVATE ${IMGUI_SOURCES_DIR}
    PRIVATE ${IMGUI_SOURCES_DIR}/backends
    PRIVATE ${IMGUI_FILE_DIALOG_DIR}
    PRIVATE ${Boost_INCLUDE_DIRS}
)

# On Windows MSYS2 link libraries statically to the binary. This way binary can
# be more easily distributed and does not depend on MSY2 being installed. On
# Linux just load used shared libraries instead.
if (MSYS2)
    target_link_libraries(${EXECUTABLE} ${SDL2_STATIC_LIBRARIES} ws2_32 -static)
else()
    target_link_libraries(${EXECUTABLE} ${SDL2_LIBRARIES})
endif()
target_link_libraries(${EXECUTABLE} OpenGL::GL ${Boost_LIBRARIES})

target_compile_options(${EXECUTABLE} PUBLIC ${SDL2_CFLAGS_OTHER})

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

enable_testing()

add_subdirectory(test)
