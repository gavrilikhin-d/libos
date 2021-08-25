cmake_minimum_required(VERSION 3.16)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# All os headers
set(
    os_headers
        ${PROJECT_SOURCE_DIR}/include/os/info.hpp
        ${PROJECT_SOURCE_DIR}/include/os/kernel.hpp
        ${PROJECT_SOURCE_DIR}/include/os/keyboard.hpp
        ${PROJECT_SOURCE_DIR}/include/os/macros.h
        ${PROJECT_SOURCE_DIR}/include/os/os.hpp
        ${PROJECT_SOURCE_DIR}/include/os/version.hpp
)

if (UNIX)
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

if (UNIX)
    find_package( X11 REQUIRED )

    # Library to manipulate GUI on linux
    target_link_libraries(os PUBLIC X11::Xtst X11::X11)
endif()

# Specify directories which the compiler should look for headers
target_include_directories(os PUBLIC ${PROJECT_SOURCE_DIR}/include)

# IDEs should put the headers in a nice place
source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)

# install library itself
install(TARGETS os LIBRARY DESTINATION lib)

# Also install the headers
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/os DESTINATION include)