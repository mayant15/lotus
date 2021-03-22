set(EDITOR_LINK_LIBS
    glad
    CONAN_PKG::glfw
)

set(EDITOR_INCLUDE_DIRS
    external
    external/nativefiledialog/include
    src
)


set(LOTUS_EDITOR_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp

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

    src/ModuleLoader.cpp
)

# TODO: Use NFD only on windows for now
if (WIN32)
    list(APPEND LOTUS_EDITOR_SOURCES external/nativefiledialog/nfd_common.c)
    list(APPEND LOTUS_EDITOR_SOURCES external/nativefiledialog/nfd_win.cpp)
endif()

# Define the target
set(LOTUS_EDITOR_EXEC editor)
add_executable(${LOTUS_EDITOR_EXEC} ${LOTUS_EDITOR_SOURCES})
add_dependencies(${LOTUS_EDITOR_EXEC} lotus)
target_include_directories(${LOTUS_EDITOR_EXEC} PRIVATE ${EDITOR_INCLUDE_DIRS})
target_link_libraries(${LOTUS_EDITOR_EXEC} lotus ${EDITOR_LINK_LIBS})
target_compile_definitions(${LOTUS_EDITOR_EXEC} PRIVATE
    ENTT_API_IMPORT
    EDITOR_RESOURCE_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)
