include(GenerateExportHeader)

# Public headers for the library
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/lotus/internal)

# Internal headers
include_directories(include)
#include_directories(ext/glfw/include)

# pybind
add_subdirectory(ext/pybind11)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(ext/glfw)

# Assimp
add_subdirectory(ext/assimp)

# spdlog
add_subdirectory(ext/spdlog)

set(SOURCES
        src/Engine.cpp

        src/events/GLWindow.cpp
        src/events/Input.cpp

        src/physics/math/operations.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

        src/rendering/opengl/glad.c
        src/rendering/opengl/GLRenderer.cpp
        src/rendering/opengl/GLShader.cpp
        src/rendering/opengl/GLTextureLoader.cpp
        
        src/resources/Shader.cpp
        src/resources/stb_image.cpp
        src/resources/Model.cpp
        src/resources/Texture.cpp
        src/resources/Cubemap.cpp
        src/resources/Material.cpp

        src/scene/Scene.cpp
        src/scene/AActor.cpp
        src/scene/ACamera.cpp
        src/scene/SceneManager.cpp
)

add_library(lotus SHARED ${SOURCES} "src/resources/Material.cpp")
target_link_libraries(lotus pybind11::embed glfw assimp spdlog::spdlog)
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)
generate_export_header( lotus
          BASE_NAME lotus
          EXPORT_MACRO_NAME LOTUS_API
)
