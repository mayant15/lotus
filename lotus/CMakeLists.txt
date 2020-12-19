# Dependencies
set(LOTUS_LINK_LIBS
    CONAN_PKG::glfw
    CONAN_PKG::assimp
    CONAN_PKG::physx
    CONAN_PKG::spdlog
)

# Other include directories
set(LOTUS_INCLUDE_DIRS
    ${PUBLIC_INCLUDE_DIR}
    ${PUBLIC_INCLUDE_DIR}/lotus/internal
    src
    external
)

# Set all source files
set(LOTUS_SOURCES
    external/glad.c
    external/stb_image.cpp

    src/lotus.cpp
    src/filesystem.cpp

    src/core/platform/DXWindow.cpp
    src/core/platform/GLWindow.cpp
    src/core/Engine.cpp
    src/core/Input.cpp
    src/core/Config.cpp

    src/ecs/SystemRegistry.cpp
    src/ecs/Entity.cpp

    src/physics/PxImpl.cpp
    src/physics/PhysicsSubsystem.cpp

    src/rendering/opengl/GLRenderer.cpp
    src/rendering/opengl/GLShader.cpp
    src/rendering/opengl/RHIImpl.cpp
    src/rendering/Primitives.cpp

    src/resources/HDRI.cpp
    src/resources/Material.cpp
    src/resources/Model.cpp
    src/resources/Shader.cpp
    src/resources/Texture.cpp

    src/scene/SceneManager.cpp
    src/scene/SceneTreeNode.cpp
)

add_library(lotus SHARED ${LOTUS_SOURCES})
target_include_directories(lotus PRIVATE ${LOTUS_INCLUDE_DIRS})
target_link_libraries(lotus ${LOTUS_LINK_LIBS})
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)

if (LOTUS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
