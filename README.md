# LibOS
[![Linux](https://github.com/gavrilikhin-d/libos/actions/workflows/linux.yml/badge.svg)](https://github.com/gavrilikhin-d/libos/actions/workflows/linux.yml)
[![Windows](https://github.com/gavrilikhin-d/libos/actions/workflows/windows.yml/badge.svg)](https://github.com/gavrilikhin-d/libos/actions/workflows/windows.yml)
[![Documentation](https://img.shields.io/badge/docs-online-informational?label=Docs&style=flat&link=https://gavrilikhin-d.github.io/libos/)](https://gavrilikhin-d.github.io/libos/)
[![License](https://img.shields.io/github/license/gavrilikhin-d/libos?label=License)](https://github.com/gavrilikhin-d/libos/blob/master/LICENSE)

LibOS is a modern C++17 library that makes OS-specific features cross-platform.

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
- **CMake** (prefered)
- **Header-only**

### CMake

1. Download sources
   ```
   git clone https://github.com/gavrilikhin-d/libos
   ```

2. Make `build` directory in repo's root folder
   ```
   mkdir build
   cd build
   ```

3. Configure CMake <br/>
   With GUI:
   ```
   ccmake ..
   ```
   Without GUI:
   ```
   cmake -DBUILD_SHARED_LIBS:BOOL=ON -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_INSTALL_PREFIX:PATH=/usr/local ..
   ```

4. Build
   ```
   cmake --build .
   ```

5. Install
   ```
   sudo cmake --install .
   ```

### Header-only

Just `#include` files from `os/header-only` as needed.

> **NOTE:** Compile with `-std=c++17` or greater.

> **NOTE:** When compiling on Linux, link `-lX11 -lXtst` if you are using `os/header-only/keyboard.hpp`.

## Contributing

Please fork this repository and contribute back using [pull requests](https://github.com/gavrilikhin-d/libos/pulls). Features can be requested using [issues](https://github.com/gavrilikhin-d/libos/issues). All code, comments, and critiques are greatly appreciated.
