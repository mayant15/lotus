# Public headers for the library
include_directories(${CMAKE_SOURCE_DIR}/include)

# Internal headers
include_directories(include)

# Build pybind
add_subdirectory(ext/pybind11)

set(SOURCES
        src/lotus.cpp
        src/Object.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

        src/rendering/opengl/glad.c
        src/rendering/opengl/GLRenderer.cpp
        src/rendering/vulkan/VkRenderer.cpp
        src/rendering/directx/DxRenderer.cpp
        src/rendering/Shader.cpp
        src/rendering/Window.cpp

        src/resources/stb_image.cpp
        src/resources/Mesh.cpp
        src/resources/Model.cpp
        src/resources/Texture.cpp
        src/resources/AssetManager.cpp

        src/scene/Scene.cpp
        src/scene/Actor.cpp
        src/scene/Camera.cpp
)

add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus ${CONAN_LIBS} pybind11::embed)
