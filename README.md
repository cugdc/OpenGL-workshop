# OpenGL Workshop
This is a OpenGL workshop for the [CU Computer Graphics Group](https://cu-computer-graphics-group.netlify.com/).

## Build instruction

This project use C++17. A recent enough C++17 compiler is required. It also uses [CMake](https://cmake.org/) build system and [Conan](https://conan.io/) package manager. To successfully build the project, you need both tools installed. You can install both cmake and conan through python `pip`:

``` shell
# Or pip3 on some linux distributions, use sudo if nessesory
$ pip install cmake conan
```

Adter installing all the tools, build the project with the following CMake instructions.
``` shell
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

Using IDE or editor with CMake support (e.g. CLion, Qt Creator, or VS Code) can gives you a better workflow than using the command line directly.
