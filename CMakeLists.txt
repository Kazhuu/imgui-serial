cmake_minimum_required(VERSION 3.20.0)
project(imgui-glfw-template)

# Specify the C++ standard.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Setup source locations.
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(IMGUI_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/imgui)
set(IMGUI_SOURCES
    ${IMGUI_SOURCES_DIR}/imgui.cpp
    ${IMGUI_SOURCES_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCES_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCES_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCES_DIR}/imgui_widgets.cpp
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl.cpp
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.cpp
)
set(SOURCES
    ${SOURCE_DIR}/main.cpp
)
set(HEADERS
    ${IMGUI_SOURCES_DIR}/imgui.h
    ${IMGUI_SOURCES_DIR}/imconfig.h
    ${IMGUI_SOURCES_DIR}/imgui_internal.h
    ${IMGUI_SOURCES_DIR}/imstb_rectpack.h
    ${IMGUI_SOURCES_DIR}/imstb_textedit.h
    ${IMGUI_SOURCES_DIR}/imstb_truetype.h
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_sdl.h
    ${IMGUI_SOURCES_DIR}/backends/imgui_impl_opengl3.h
)

# External packages.
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

# Executable
set(EXECUTABLE imgui_example)
add_executable(${EXECUTABLE})
target_sources(${EXECUTABLE} PUBLIC ${HEADERS} PRIVATE ${IMGUI_SOURCES} ${SOURCES})
target_include_directories(
    ${EXECUTABLE}
    PRIVATE ${IMGUI_SOURCES_DIR}
    PRIVATE ${IMGUI_SOURCES_DIR}/backends
)
target_link_libraries(${EXECUTABLE} SDL2::SDL2 OpenGL::GL)

# Enable warnings.
if(MSVC)
    target_compile_options(${EXECUTABLE} PRIVATE /W4 /WX)
else()
    target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
