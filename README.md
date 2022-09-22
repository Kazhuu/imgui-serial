# imgui-sdl-template

IMGUI + SDL2 + OpenGL 3 template project with both CMake and Makefile support
for Windows and Linux machines.

This template is based on
[this IMGUI example](https://github.com/ocornut/imgui/tree/master/examples/example_sdl_opengl3).

## Prerequisites

After cloning this repository, you need to clone IMGUI as a Git submodule, by
default submodules are not cloned initially. To do this after cloning in the
project directory run

```shell
git submodule init
git submodule udate
```

You need to install SDL2 on your system. On Arch Linux for example

```shell
sudo pacman -S sdl sdl2
```

And on Windows MSYS2/MinGW do

```shell
pacman -S mingw-w64-i686-SDL2
```

You system also need to have OpenGL drivers installed for your GPU. On Linux
[Mesa](https://mesa3d.org/) for example. Check your systems package manager for
it.

## Build

### CMake

#### Windows MSYS2

If you are using Windows and MSYS2. Make sure you install MinGW version of CMake
`mingw-w64-x86_64-cmake`. This will ensure that you are building for Windows.
You also need to install the tool for actual building. By default Ninja is used
if generator for the CMake is not specified. Intall Ninja from
`mingw-w64-x86_64-ninja`. For futher information check
[CMake forMSYS2](https://www.msys2.org/docs/cmake/).

Install needed packages with

```
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

Then just run following to build. If you want release build add
`-DCMAKE_BUILD_TYPE=Release` to first CMake command.

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

#### Linux

To build run following commands. If you want release build add
`-DCMAKE_BUILD_TYPE=Release` to first CMake command.

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

### Makefile

Or with Make

```shell
make
```

## Running

After everything compiles, executable `imgui_example` will be placed under your
`build` directory. To run it in the build folder

```shell
./imgui_example
```
