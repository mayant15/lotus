set(LOTUS_BUILDTOOL_NAME buildtool)

set(LOTUS_BUILDTOOL_INCLUDE_DIRS
    PRIVATE src
    PUBLIC include
)

set(LOTUS_BUILDTOOL_SOURCES
    src/main.cpp
)

add_executable(${LOTUS_BUILDTOOL_NAME} ${LOTUS_BUILDTOOL_SOURCES})
add_dependencies(${LOTUS_BUILDTOOL_NAME} lotus)
target_include_directories(${LOTUS_BUILDTOOL_NAME} PRIVATE ${LOTUS_BUILDTOOL_INCLUDE_DIRS})
target_link_libraries(${LOTUS_BUILDTOOL_NAME} lotus)
target_compile_definitions(${LOTUS_BUILDTOOL_NAME} PRIVATE ENTT_API_IMPORT)
