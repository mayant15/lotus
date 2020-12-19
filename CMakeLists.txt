cmake_minimum_required(VERSION 3.13)
project(lotus)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Clean, organized outputs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build options
option(LOTUS_BUILD_TESTS "Build unit tests for the library" ON)
option(LOTUS_BUILD_EXAMPLE "Build an example demonstrating the usage of the engine" ON)

# Things like examples also need public headers for lotus
set(PUBLIC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lotus/include) 

# The engine needs access to the engine resource root
# The user project needs access to both the engine and the user resource root
# So I enabled this definition globally for all targets
add_compile_definitions(ENGINE_RESOURCE_ROOT="${CMAKE_SOURCE_DIR}/lotus/resources/")

# Build the actual engine
add_subdirectory(lotus)

if (LOTUS_BUILD_EXAMPLE)
    add_subdirectory(examples/quickstart)
endif()
