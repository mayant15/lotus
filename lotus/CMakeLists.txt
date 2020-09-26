# Include paths
include_directories(${PUBLIC_INCLUDE_DIR})
include_directories(src)
include_directories(external)

# Dependencies
find_package(Assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
find_package(PhysX REQUIRED)


set(SOURCES
        external/glad.c
        external/stb_image.cpp

        src/Engine.cpp

        src/events/GLWindow.cpp
        src/events/Input.cpp

        src/physics/math/operations.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

        src/rendering/opengl/GLRenderer.cpp
        src/rendering/opengl/GLShader.cpp
        src/rendering/opengl/GLTextureLoader.cpp
        
        src/resources/Shader.cpp
        src/resources/Model.cpp
        src/resources/Texture.cpp
        src/resources/Cubemap.cpp
        src/resources/Material.cpp

        src/scene/Scene.cpp
        src/scene/AActor.cpp
        src/scene/ACamera.cpp
        src/scene/SceneManager.cpp
)
add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus PRIVATE pybind11::embed glfw spdlog::spdlog assimp::assimp PhysX)
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)

if (LOTUS_BUILD_DOCS)
    add_subdirectory(docs)
endif()
