cmake_minimum_required(VERSION 3.16)

# where to store executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Copy dll to executables location
if (WIN32 AND BUILD_SHARED_LIBS)
    file(COPY "${CMAKE_BINARY_DIR}/lib/os.dll" DESTINATION ${EXECUTABLE_OUTPUT_PATH})
endif()

# macro for easy adding examples
macro(add_example _NAME)
    add_executable(${_NAME} ${_NAME}.cpp)
    target_link_libraries(${_NAME} PRIVATE os)
endmacro()

add_example(keyboard)
add_example(os_and_kernel_info)