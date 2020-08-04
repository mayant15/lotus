# Public headers for the library
include_directories(${CMAKE_SOURCE_DIR}/include)

# Internal headers
include_directories(include)

# Build dependencies
add_subdirectory(ext/pybind11)
#add_subdirectory(ext/entt)

set(SOURCES
        src/lotus.cpp

        src/physics/math/operations.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

        src/rendering/opengl/glad.c
        src/rendering/opengl/GLRenderer.cpp
        src/rendering/Shader.cpp
        src/rendering/Window.cpp
        src/rendering/AModel.cpp
        src/rendering/Renderer.cpp

        src/resources/stb_image.cpp
        src/resources/Mesh.cpp
        src/resources/Model.cpp
        src/resources/Texture.cpp
        src/resources/AssetManager.cpp

        src/scene/Scene.cpp
        src/scene/Actor.cpp
        src/scene/Camera.cpp

#        src/tools/logger.cpp
)

add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus ${CONAN_LIBS} pybind11::embed)
