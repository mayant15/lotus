cmake_minimum_required(VERSION 3.16)

# vcpkg
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(lotus)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/external/cmake" ${CMAKE_MODULE_PATH})
set(PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lotus/include)

option(LOTUS_BUILD_DOCS "Build the Lotus documentation" ON)
option(LOTUS_BUILD_EXAMPLE "Build an example demonstrating the usage of the engine" ON)

# TODO: Find a better way to get shader paths
add_compile_definitions(SHADER_ROOT="${CMAKE_SOURCE_DIR}/lotus/shaders/")

# Build libraries
add_subdirectory(lotus)

if (LOTUS_BUILD_EXAMPLE)
    add_subdirectory(examples/quickstart)
endif()
