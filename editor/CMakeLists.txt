set(EDITOR_LINK_LIBS
    CONAN_PKG::glfw
)

set(EDITOR_INCLUDE_DIRS
    external
    src
    ${PUBLIC_INCLUDE_DIR}
    ${PUBLIC_INCLUDE_DIR}/lotus/internal
)

set(LOTUS_EDITOR_SOURCES
    external/glad.c
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp

    src/main.cpp
    src/window/imgui_impl_opengl3.cpp
    src/window/imgui_impl_glfw.cpp
)

# Define the target
set(LOTUS_EDITOR_EXEC editor)
add_executable(${LOTUS_EDITOR_EXEC} ${LOTUS_EDITOR_SOURCES})
#add_dependencies(${LOTUS_EDITOR_EXEC} lotus)
target_include_directories(${LOTUS_EDITOR_EXEC} PRIVATE ${EDITOR_INCLUDE_DIRS})
target_link_libraries(${LOTUS_EDITOR_EXEC} ${EDITOR_LINK_LIBS})
target_compile_definitions(${LOTUS_EDITOR_EXEC} PRIVATE
    ENTT_API_IMPORT
    RESOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)
