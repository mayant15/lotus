include(GenerateExportHeader)

# Public headers for the library
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/lotus/internal)

# Internal headers
include_directories(include)

# Dependencies
find_package(Assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

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
add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus PRIVATE pybind11::embed glfw spdlog::spdlog assimp::assimp)
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)
generate_export_header( lotus
          BASE_NAME lotus
          EXPORT_MACRO_NAME LOTUS_API
)
