include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(example main.cpp)
add_dependencies(example lotus)
target_link_libraries(example lotus ${CONAN_LIBS})
