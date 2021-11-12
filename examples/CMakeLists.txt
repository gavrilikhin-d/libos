cmake_minimum_required(VERSION 3.16)

# where to store executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# macro for easy adding examples
macro(add_example _NAME)
    add_executable(${_NAME} ${_NAME}.cpp)
    target_link_libraries(${_NAME} PRIVATE os)
    # Copy dll to executables location
    if (WIN32 AND BUILD_SHARED_LIBS)
        add_custom_command(
            TARGET ${_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:os> $<TARGET_FILE_DIR:${_NAME}>
        )
    endif()
endmacro()

add_example(keyboard)
add_example(os_and_kernel_info)

# Add header-only library test
add_example(header-only)