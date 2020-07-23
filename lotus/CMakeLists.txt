include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
        lotus.cpp
        Object.cpp

        rendering/opengl/glad.c
        rendering/opengl/GLRenderer.cpp
        rendering/vulkan/VkRenderer.cpp
        rendering/directx/DxRenderer.cpp
        rendering/Shader.cpp
        rendering/Window.cpp
        rendering/debug.hpp

        resources/stb_image.cpp
        resources/Mesh.cpp
        resources/Model.cpp
        resources/Texture.cpp
        resources/AssetManager.cpp

        scene/Scene.cpp
        scene/Actor.cpp
        scene/Camera.cpp

#        tools/GUI.cpp
#        tools/GLUIRenderBackend.cpp
)

add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus ${CONAN_LIBS})
