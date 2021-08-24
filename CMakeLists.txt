cmake_minimum_required(VERSION 3.16)

project(
    libos
        VERSION "0.1.0"
        DESCRIPTION "Cross-platform OS features in C++"
        HOMEPAGE_URL https://github.com/gavrilikhin-d/libos
        LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(src)