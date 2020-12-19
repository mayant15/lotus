# lotus include paths
set(EXAMPLE_INCLUDE_DIRS
    ${PUBLIC_INCLUDE_DIR}
    ${PUBLIC_INCLUDE_DIR}/lotus/internal
)

# Configure setup files
set(EXAMPLE_CACHE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.settings)
set(DEFAULT_PROJECT_RESOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/resources/)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.lproject ${EXAMPLE_CACHE_DIR}/.lproject @ONLY)

# Define the target
add_executable(example main.cpp)
add_dependencies(example lotus)
target_include_directories(example PRIVATE ${EXAMPLE_INCLUDE_DIRS})
target_link_libraries(example lotus)
target_compile_definitions(example PRIVATE
    PROJECT_SETTINGS=\"${EXAMPLE_CACHE_DIR}/.lproject\"
    ENTT_API_IMPORT
)
