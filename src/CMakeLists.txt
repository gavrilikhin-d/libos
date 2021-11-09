cmake_minimum_required(VERSION 3.16)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# All os headers
set(
    os_headers
        ${PROJECT_SOURCE_DIR}/include/os/info.hpp
        ${PROJECT_SOURCE_DIR}/include/os/kernel.hpp
        ${PROJECT_SOURCE_DIR}/include/os/keyboard.hpp
        ${PROJECT_SOURCE_DIR}/include/os/libos.hpp
        ${PROJECT_SOURCE_DIR}/include/os/macros.h
        ${PROJECT_SOURCE_DIR}/include/os/os.hpp
        ${PROJECT_SOURCE_DIR}/include/os/version.hpp
)

if (APPLE)
    # Add macOS sources
    set(
        os_sources
            macos/info.cpp
            macos/kernel.cpp
            macos/keyboard.cpp
    )
elseif (UNIX)
    # Add linux sources
    set(
        os_sources
            linux/info.cpp
            linux/kernel.cpp
            linux/keyboard.cpp
    )
endif()

if (WIN32)
    # Add windows sources
    set(
        os_sources
            windows/info.cpp
            windows/kernel.cpp
            windows/keyboard.cpp
    )

    # Link dynamic library on Windows
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_library(os ${os_sources} ${os_headers})

# Requires at least C++17
target_compile_features(os PUBLIC cxx_std_17)

if (APPLE)
    find_library(CF CoreFoundation)
    if (NOT CF)
        message(FATAL_ERROR "CoreFoundation not found")
    endif()

    target_link_libraries(os ${CF})
elseif (UNIX)
    find_package( X11 REQUIRED )

    # Library to manipulate GUI on linux
    target_link_libraries(os PUBLIC X11::Xtst X11::X11)
endif()

# Specify directories which the compiler should look for headers
target_include_directories(os PUBLIC ${PROJECT_SOURCE_DIR}/include)

# install library itself
install(TARGETS os LIBRARY DESTINATION lib)

# Also install the headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/os DESTINATION include)