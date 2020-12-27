#include "RHI.h"
#include "opengl/RHIImpl.h"

#include <lotus/Config.h>

namespace Lotus::RHI
{
    /** @brief Macro to simplify implementation. Passes arguments as is to API-specific implementation. */
#define DELEGATE_TO_API(func, ...) \
    auto conf = GetRenderConfig(); \
    switch (conf.RenderAPI) { \
    case ERenderAPI::OPEN_GL: return Detail::GL##func (__VA_ARGS__); \
    default: LOG_ERROR("Only OpenGL is supported."); }

    void PlatformInit() { DELEGATE_TO_API(PlatformInit) }

    void EnableDebugContext() { DELEGATE_TO_API(EnableDebugContext) }

    void EnableFeatures(unsigned int bits) { DELEGATE_TO_API(EnableFeatures, bits) }

    void SetClearColor(float r, float g, float b, float a) { DELEGATE_TO_API(SetClearColor, r, g, b, a) }

    void SetDepthFunction(unsigned int func) { DELEGATE_TO_API(SetDepthFunction, func) }

    unsigned int GetTexture2DTarget() { DELEGATE_TO_API(GetTexture2DTarget) }

    void SetCullFace(unsigned int face) { DELEGATE_TO_API(SetCullFace, face) }

    TextureID CreateTexture(const TextureInfo& info) { DELEGATE_TO_API(CreateTexture, info) }

    CubeMap CreateCubeMap(const TextureInfo& info) { DELEGATE_TO_API(CreateCubeMap, info) }

    void SetActiveTextureSlot(TextureSlot slot) { DELEGATE_TO_API(SetActiveTextureSlot, slot) }

    void BindTexture(TextureID id) { DELEGATE_TO_API(BindTexture, id) }

    void BindCubeMap(TextureID id) { DELEGATE_TO_API(BindCubeMap, id) }

    unsigned int GetCubeMapPositiveX() { DELEGATE_TO_API(GetCubeMapPositiveX) }

    void Clear(unsigned int bits) { DELEGATE_TO_API(Clear, bits) }

    FrameBuffer CreateFrameBuffer(const FrameBufferInfo& info) { DELEGATE_TO_API(CreateFrameBuffer, info) }

    void AttachRenderBuffer(FrameBuffer fb, const RenderBufferAttachmentInfo& info) { DELEGATE_TO_API(AttachRenderBuffer, fb, info) }

    void AttachTexture(FrameBuffer fb, const TextureAttachmentInfo& info) { DELEGATE_TO_API(AttachTexture, fb, info) }

    void BindFrameBuffer(FrameBuffer fbo) { DELEGATE_TO_API(BindFrameBuffer, fbo) }

    VAO CreateVAO(const VAOInfo& info) { DELEGATE_TO_API(CreateVAO, info) }

    void BindVAO(VAO vao) { DELEGATE_TO_API(BindVAO, vao) }

    void SetViewport(unsigned int width, unsigned int height) { DELEGATE_TO_API(SetViewport, width, height) }

    void DrawTriangles(unsigned int count) { DELEGATE_TO_API(DrawTriangles, count) }

    void DrawElements(unsigned int size, const void* indices) { DELEGATE_TO_API(DrawElements, size, indices) }

    void CreateBuffersForMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int* vao, unsigned int* vbo, unsigned int* ebo)
    {
        DELEGATE_TO_API(CreateBuffersForMesh, vertices, indices, vao, vbo, ebo);
    }
}
