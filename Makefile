# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL2

#CXX = g++
#CXX = clang++

BUILD_DIR := build
SOURCE_DIR := src
IMGUI_DIR := ext/imgui
EXEC := imgui_example

SOURCES := $(shell find $(SOURCE_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJS := $(SOURCES:%=$(BUILD_DIR)/%.o)

UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat -MMD -MP
LIBS =

ifeq ($(UNAME_S), Linux)
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW"
    LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

    CXXFLAGS += `pkg-config --cflags sdl2`
    CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

$(BUILD_DIR)/$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)
	@echo Build complete for $(ECHO_MESSAGE)

$(BUILD_DIR)/%.cpp.o:%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf build

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
