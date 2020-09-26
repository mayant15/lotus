
# lotus include paths
include_directories(${PUBLIC_INCLUDE_DIR})

# Header only libraries need to be present on the include path while building the examples
include_directories(${PUBLIC_INCLUDE_DIR}/lotus/internal)

add_executable(example main.cpp)
add_dependencies(example lotus)
target_link_libraries(example lotus)

target_compile_definitions(example PRIVATE 
    ENTT_API_IMPORT 
    DEFAULT_RESOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)
