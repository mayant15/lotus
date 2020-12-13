# lotus include paths
set(LOTUS_TEST_INCLUDE_DIRS
        ${PUBLIC_INCLUDE_DIR}
        ${PUBLIC_INCLUDE_DIR}/lotus/internal
)

set(LOTUS_TEST_EXEC tests)

set(LOTUS_TEST_SOURCES
    doctest.cpp
    filesystem.cpp
)

add_executable(${LOTUS_TEST_EXEC} ${LOTUS_TEST_SOURCES})
add_dependencies(${LOTUS_TEST_EXEC} lotus)
target_include_directories(${LOTUS_TEST_EXEC} PRIVATE ${LOTUS_TEST_INCLUDE_DIRS})
target_link_libraries(${LOTUS_TEST_EXEC} lotus)
target_compile_definitions(${LOTUS_TEST_EXEC} PRIVATE ENTT_API_IMPORT)
