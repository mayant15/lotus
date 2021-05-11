# Define the target
set(EXAMPLE_PONG_SOURCES
    src/pong.cpp
)

add_library(pong SHARED ${EXAMPLE_PONG_SOURCES})
add_dependencies(pong lotus)
target_include_directories(pong
    PUBLIC include
    PRIVATE src
)
target_link_libraries(pong lotus)
target_compile_definitions(pong PRIVATE ENTT_API_IMPORT)
