# LibOS
[![Linux](https://github.com/gavrilikhin-d/libos/actions/workflows/linux.yml/badge.svg)](https://github.com/gavrilikhin-d/libos/actions/workflows/linux.yml)
[![Windows](https://github.com/gavrilikhin-d/libos/actions/workflows/windows.yml/badge.svg)](https://github.com/gavrilikhin-d/libos/actions/workflows/windows.yml)
[![Documentation](https://img.shields.io/badge/docs-online-informational?label=Docs&style=flat&link=https://gavrilikhin-d.github.io/libos/)](https://gavrilikhin-d.github.io/libos/)
[![License](https://img.shields.io/github/license/gavrilikhin-d/libos?label=License)](https://github.com/gavrilikhin-d/libos/blob/master/LICENSE)

Cross-platform OS features in C++17.

Ever tried to get Windows version after Windows 8? Or to send keystrokes on all most popular platforms? Then you must understand what a pain it is.
Those wondering *"What the hell is an X server and do I have it???"* welcome to use our library!

## Features

- OS and Kernel Info
- Keyboard Input

## Documentation

Documentation can be found [here](https://gavrilikhin-d.github.io/libos/). Work in progress.

## Dependencies

If you use keyboard features on Linux, you need to install `libxtst-dev`:
```
sudo apt-get install libxtst-dev
```

P.S: most likely it's already installed

## Getting started

There are 2 ways to install the library for your convenience:
1. **CMake** (prefered)
2. **Header-only**

### CMake

Download sources:
```
git clone https://github.com/gavrilikhin-d/libos
```

Build and install:
```
cd os
mkdir build
cd build
cmake ..
cmake --build .
cmake --install .
```

### Header-only

Download `include/os/header-only/os.hpp` and `#include` it.

> **NOTE:** Don't forget to compile with `-std=c++17` or greater, because it's C++17 library.

> **NOTE:** When compiling on Linux, don't forget to link `-lX11` and `-lXtst`.

## Contributing

Please fork this repository and contribute back using [pull requests](https://github.com/gavrilikhin-d/libos/pulls). Features can be requested using [issues](https://github.com/gavrilikhin-d/libos/issues). All code, comments, and critiques are greatly appreciated.
