# Build dependencies
add_subdirectory(external/assimp)
add_subdirectory(external/entt)
add_subdirectory(external/glm)
add_subdirectory(external/nlohmann)
add_subdirectory(external/stb_image)
# add_subdirectory(external/physx)

# Other include directories

# set(LOTUS_PUBLIC_INCLUDE_DIRS
#     include
#     include/lotus/internal
# )

# set(LOTUS_PRIVATE_INCLUDE_DIRS
#     src
#     external
# )

# # Set all source files
# set(LOTUS_SOURCES
#     external/stb_image.cpp

#     src/filesystem.cpp

#     src/core/Engine.cpp
#     src/core/Input.cpp
#     src/core/Config.cpp
#     src/core/ComponentRegistry.cpp

#     src/physics/PxImpl.cpp
#     src/physics/Physics.cpp

#     src/rendering/opengl/GLShader.cpp
#     src/rendering/debug/ShaderHotReload.cpp
#     src/rendering/Primitives.cpp
#     src/rendering/Renderer.cpp
#     src/rendering/RHI.cpp

#     src/resources/AssetRegistry.cpp
#     src/resources/HDRI.cpp
#     src/resources/Material.cpp
#     src/resources/Model.cpp
#     src/resources/Shader.cpp
#     src/resources/Texture.cpp

#     src/scene/Scene.cpp
#     src/scene/SceneManager.cpp
#     src/scene/SceneTreeNode.cpp
# )

# add_library(lotus SHARED ${LOTUS_SOURCES})
# add_dependencies(lotus glad)
# target_include_directories(lotus
#     PUBLIC ${LOTUS_PUBLIC_INCLUDE_DIRS}
#     PRIVATE ${LOTUS_PRIVATE_INCLUDE_DIRS}
# )
# target_link_libraries(lotus ${LOTUS_LINK_LIBS})
# target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)

# if (LOTUS_BUILD_TESTS)
#     add_subdirectory(tests)
# endif()
