set(LOTUS_TEST_SOURCES
    doctest.cpp

    Config.cpp
    filesystem.cpp
)

add_executable(tests ${LOTUS_TEST_SOURCES})
add_dependencies(tests lotus)
target_link_libraries(tests lotus)
target_compile_definitions(tests PRIVATE
    ENTT_API_IMPORT
    PROJECT_SETTINGS=\"${CMAKE_CURRENT_SOURCE_DIR}/project/.lproject\"
)
