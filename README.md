# imgui-serial

IMGUI + SDL2 serial port application for [SPC
Player](https://github.com/Kazhuu/spc-player) Arduino project. This project is a
work in progress! Project supports both Linux and Windows and on Windows
compilation requires MSYS2 environment.

## Prerequisites

After cloning this repository, you need to clone the Git submodules, by default
submodules are not cloned initially. Do this by running following in a project
directory:

```shell
git submodule init
git submodule udate
```

You system also need to have OpenGL drivers installed for your GPU. On Linux
[Mesa](https://mesa3d.org/) for example. Check your systems package manager for
it. Usually these are already installed some way or other and no need to install
anything.

This project uses [Conan](https://conan.io/) package manager to manage it's
dependencies where possible. So make sure to install it and the `conan` command
is working for you.

## Build

### Windows MSYS2/MinGW

On Windows I have tested this project with MinGW64 shell, that is shipped with
MSYS2 installation.

First you need to install SDL2 and Boost libraries. To do that run:

```shell
pacman -S mingw-w64-i686-SDL2 mingw-w64-x86_64-boost
```

When building with MSYS2. Make sure you install MinGW version of CMake
`mingw-w64-x86_64-cmake`. This will ensure that you are building for Windows.
You also need to install the tool for actual building. By default Ninja is used
if generator for the CMake is not specified. Install Ninja from
`mingw-w64-x86_64-ninja`. For further information check
[CMake forMSYS2](https://www.msys2.org/docs/cmake/).

Install above packages with:

```shell
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

To build the application, run following commands in the project folder. Debug
build is compiled by default, if you want a release build, please add
`-DCMAKE_BUILD_TYPE=Release` to first CMake command. The flag `-b` tells Conan
to compile the dependencies if no precompiled binaries are found.

```shell
mkdir build && cd build
conan install -b ..
cmake ..
cmake --build .
```

### Linux

You need to install SDL2 and Boost libraries for your system. On Arch Linux for
example:

```shell
sudo pacman -S sdl sdl2 boost
```

To build the application, run following commands in the project folder. Debug
build is compiled by default, if you want a release build, please add
`-DCMAKE_BUILD_TYPE=Release` to first CMake command. The flag `-b` tells Conan
to compile the dependencies if no precompiled binaries are found.

```shell
mkdir build && cd build
conan install -b ..
cmake ..
cmake --build .
```

## Running

After everything compiles, executable `spc_player` will be placed under your
`build/bin` directory. Run it from the build folder with:

```shell
./bin/spc_player
```

## Tests

After successful build, in the build folder simple execute:

```shell
ctest
```

Another way is to execute the test are the other executables under `build/bin` folder.
