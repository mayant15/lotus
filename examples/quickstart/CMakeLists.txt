# lotus include paths
set(EXAMPLE_INCLUDE_DIRS
    ${PUBLIC_INCLUDE_DIR}
    ${PUBLIC_INCLUDE_DIR}/lotus/internal
)

add_executable(example main.cpp)
add_dependencies(example lotus)
target_include_directories(example PRIVATE ${EXAMPLE_INCLUDE_DIRS})
target_link_libraries(example lotus)

target_compile_definitions(example PRIVATE 
    ENTT_API_IMPORT
    DEFAULT_PROJECT_RESOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)
