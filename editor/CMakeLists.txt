# Build external dependencies
add_subdirectory(external/glfw)
add_subdirectory(external/imgui)
add_subdirectory(external/nativefiledialog)
add_subdirectory(external/IconsForkAwesome)

set(LOTUS_EDITOR_DEPS
    glad
    spdlog
    glfw
    imgui
    lotus
    icons_fork_awesome
)

set(LOTUS_EDITOR_SOURCES
    src/main.cpp
    src/utils.cpp
    src/EditorCamera.cpp

    src/window/Window.cpp
    src/window/backends/imgui_impl_opengl3.cpp
    src/window/backends/imgui_impl_glfw.cpp

    src/widgets.cpp
    src/widgets/viewport.cpp
    src/widgets/mainmenu.cpp
    src/widgets/log.cpp
    src/widgets/scenetree.cpp
    src/widgets/control.cpp

    src/gizmos.cpp

    src/ModuleLoader.cpp
)

# TODO: Use NFD only on windows for now
if (WIN32)
    list(APPEND LOTUS_EDITOR_DEPS nfd)
endif()

# Define the target
add_executable(editor ${LOTUS_EDITOR_SOURCES})
add_dependencies(editor ${LOTUS_EDITOR_DEPS})
target_include_directories(editor PRIVATE src)
target_link_libraries(editor PRIVATE ${LOTUS_EDITOR_DEPS})
target_compile_definitions(editor PRIVATE
    ENTT_API_IMPORT
    EDITOR_RESOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)
