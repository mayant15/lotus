set(QUICKSTART_SOURCES
    src/quickstart.cpp
    src/GameCamera.cpp
)

add_library(quickstart SHARED ${QUICKSTART_SOURCES})
add_dependencies(quickstart lotus)
target_include_directories(quickstart PRIVATE include)
target_link_libraries(quickstart lotus)
target_compile_definitions(quickstart PRIVATE ENTT_API_IMPORT)
