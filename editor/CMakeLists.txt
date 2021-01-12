# lotus include paths
set(EDITOR_INCLUDE_DIRS
    ${PUBLIC_INCLUDE_DIR}
    ${PUBLIC_INCLUDE_DIR}/lotus/internal
)

set(LOTUS_EDITOR_SOURCES
    src/main.cpp
)

# Define the target
set(LOTUS_EDITOR_EXEC editor)
add_executable(${LOTUS_EDITOR_EXEC} ${LOTUS_EDITOR_SOURCES})
add_dependencies(${LOTUS_EDITOR_EXEC} lotus)
target_include_directories(${LOTUS_EDITOR_EXEC} PRIVATE ${EDITOR_INCLUDE_DIRS})
target_link_libraries(${LOTUS_EDITOR_EXEC} lotus)
target_compile_definitions(${LOTUS_EDITOR_EXEC} PRIVATE ENTT_API_IMPORT)
