include_directories(${CMAKE_SOURCE_DIR}/include)

# Improve this to not use glob?
set(SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/stb_image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/shaders/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/GLRenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/Mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/rendering/Model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/GLUIRenderBackend.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/GUI.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/VkUIRenderBackend.cpp
)

add_library(lotus SHARED ${SOURCES})
target_link_libraries(lotus ${CONAN_LIBS})
