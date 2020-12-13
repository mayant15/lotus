# Dependencies
find_package(Assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# PhysX does not export its targets, like others do. Manually find files.
set(VCPKG_LIB_PATHS_DEBUG
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/debug/lib
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-linux/debug/lib
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-osx/debug/lib
)

set(VCPKG_LIB_PATHS_RELEASE
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/lib
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-linux/lib
    ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-osx/lib
)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(PHYSX_LIB_DIR ${VCPKG_LIB_PATHS_RELEASE})
    message("-- Using release PhysX")
else()
    set(PHYSX_LIB_DIR ${VCPKG_LIB_PATHS_DEBUG})
    message("-- Using debug PhysX")
endif()

find_library(PHYSX_BASE NAMES PhysX_64 PhysX_32 PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_COMMON NAMES PhysXCommon_64 PhysXCommon_32 PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_FOUNDATION NAMES PhysXFoundation_64 PhysXFoundation_32 PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_EXTENSIONS NAMES PhysXExtensions_static_64 PhysXExtensions_static_32 PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_PVD NAMES PhysXPvdSDK_static_64 PhysXPvdSDK_static_32 PATHS ${PHYSX_LIB_DIR})
set(PHYSX_LINK_LIBS ${PHYSX_COMMON} ${PHYSX_FOUNDATION} ${PHYSX_BASE} ${PHYSX_EXTENSIONS} ${PHYSX_PVD})

set(LOTUS_PRIVATE_DEPENDENCIES pybind11::embed glfw assimp::assimp ${PHYSX_LINK_LIBS})
set(LOTUS_PUBLIC_DEPENDENCIES spdlog::spdlog)

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
        src/core/IWindow.cpp

        src/ecs/SystemRegistry.cpp
        src/ecs/Entity.cpp

        src/physics/PxImpl.cpp
        src/physics/PhysicsSubsystem.cpp

        src/python/Interpreter.cpp
        src/python/module.cpp

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
target_link_libraries(lotus PUBLIC ${LOTUS_PUBLIC_DEPENDENCIES} PRIVATE ${LOTUS_PRIVATE_DEPENDENCIES})
target_compile_definitions(lotus PRIVATE ENTT_API_EXPORT)

if (LOTUS_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if (LOTUS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
