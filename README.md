# imgui-sdl-template

IMGUI + SDL2 + OpenGL 3 template project with both CMake and Makefile support
for Windows and Linux machines.

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

Build with CMake

```shell
mkdir build && cd build
cmake ..
cmake --build .
```

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
