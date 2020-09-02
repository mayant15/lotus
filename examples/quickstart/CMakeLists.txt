include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/lotus/internal)
add_executable(example main.cpp)
add_dependencies(example lotus) # in an actual app you would have to find_library lotus and then link below
target_link_libraries(example lotus)
target_compile_definitions(example PRIVATE ENTT_API_IMPORT)
