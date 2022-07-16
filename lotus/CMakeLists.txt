# Build dependencies
add_subdirectory(external/assimp)
add_subdirectory(external/entt)
add_subdirectory(external/glm)
add_subdirectory(external/nlohmann)
add_subdirectory(external/stb_image)
# add_subdirectory(external/physx)

# Dependencies for this library target
set(LOTUS_PUBLIC_DEPS
    entt
    glm
    nlohmann_json
)

set(LOTUS_PRIVATE_DEPS
    glad
    spdlog
    assimp
    stb_image
)

# Set all source files
set(LOTUS_SOURCES
    src/filesystem.cpp

    src/core/Engine.cpp
    src/core/Input.cpp
    src/core/Config.cpp
    src/core/ComponentRegistry.cpp

    src/physics/PxImpl.cpp
    src/physics/Physics.cpp

    src/rendering/opengl/GLShader.cpp
    src/rendering/debug/ShaderHotReload.cpp
    src/rendering/Primitives.cpp
    src/rendering/Renderer.cpp
    src/rendering/RHI.cpp

    src/resources/AssetRegistry.cpp
    src/resources/HDRI.cpp
    src/resources/Material.cpp
    src/resources/Model.cpp
    src/resources/Shader.cpp
    src/resources/Texture.cpp

    src/scene/Scene.cpp
    src/scene/SceneManager.cpp
    src/scene/SceneTreeNode.cpp
)

add_library(lotus SHARED ${LOTUS_SOURCES})
add_dependencies(lotus ${LOTUS_PUBLIC_DEPS} ${LOTUS_PRIVATE_DEPS})
target_include_directories(lotus
    PUBLIC include
    PRIVATE src
)
target_link_libraries(lotus PUBLIC ${LOTUS_PUBLIC_DEPS} PRIVATE ${LOTUS_PRIVATE_DEPS})
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)

# if (LOTUS_BUILD_TESTS)
#     add_subdirectory(tests)
# endif()
