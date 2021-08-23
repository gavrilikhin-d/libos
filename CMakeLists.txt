cmake_minimum_required(VERSION 3.16)

project(
    libos
        VERSION "0.1.0"
        DESCRIPTION "C++ OS library"
        HOMEPAGE_URL https://github.com/gavrilikhin-d/os
        LANGUAGES CXX
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)


add_subdirectory(src)