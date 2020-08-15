# Public headers for the library
include_directories(${CMAKE_SOURCE_DIR}/include)

# Internal headers
include_directories(include)
#include_directories(ext/glfw/include)

# pybind
add_subdirectory(ext/pybind11)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(ext/glfw)

set(SOURCES
        src/Engine.cpp

        src/events/Event.cpp
        src/events/GLWindow.cpp
        src/events/Input.cpp

        src/physics/math/operations.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

        src/rendering/LShader.cpp
        src/rendering/opengl/glad.c
        src/rendering/opengl/GLRenderer.cpp

        src/resources/stb_image.cpp
        src/resources/Mesh.cpp
        src/resources/Model.cpp
        src/resources/Texture.cpp
        src/resources/AssetManager.cpp

        src/scene/Scene.cpp
        src/scene/AActor.cpp
        src/scene/ACamera.cpp
        src/scene/SceneManager.cpp
)

add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus ${CONAN_LIBS} pybind11::embed glfw)
