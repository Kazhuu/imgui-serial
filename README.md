# imgui-glfw-template

IMGUI + GLFW + OpenGL 3 template project with both CMake and Makefile support.

## Prerequisites

After cloning this repository, you need to clone IMGUI as a Git submodule, by
default submodules are not cloned initially. To do this after cloning in the
project directory run

```shell
git submodule init
git submodule udate
```

Install GFLW on your system. For example on Arch Linux with pacman

```shell
sudo pacman -S glfw
```

You system also need to have OpenGL drivers installed for your GPU. On Linux
[Mesa](https://mesa3d.org/) for example, which has drivers for Intel Graphics,
ATI AMD and NVIDIA GPUs. Check your systems package manager for it.

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
