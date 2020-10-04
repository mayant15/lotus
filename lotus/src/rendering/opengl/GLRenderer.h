#pragma once

#include "lotus/Renderer.h"
#include "core/platform/GLWindow.h"

#define glCheckError() glCheckError_(__FILE__, __LINE__)
GLenum glCheckError_(const char* file, int line);

namespace Lotus
{
    class GLRenderer : public Renderer, public Singleton<GLRenderer>
    {
        unsigned int _shadowFBO = 0;
        unsigned int _shadowDepthTexture = 0;
        Handle<Shader> _shadowShader;
        Handle<Shader> _skyShader;

        std::vector<PointLightInfo> ptLightParams;
        std::vector<SpotLightInfo> spLightParams;
        std::vector<LightInfo> dirLightParams;

        Matrix4f view{};
        Matrix4f projection{};
        Matrix4f lightView{};
        Matrix4f lightProjection{};
        Vector3f cameraPos{};

    private:
        friend class Singleton<GLRenderer>;
        GLRenderer() = default;

        static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar* message,
                                         const void* userParam);

    public:
        void Initialize(const RendererOp& options) override;

        void SetViewport(int x, int y, int width, int height) override;

        void DrawMesh(const CMeshRenderer& data, const CTransform& transform) override;

        void OnPreUpdate(const PreUpdateEvent& event) override;
        void OnUpdate(const UpdateEvent& event) override;

        unsigned int createTexture(unsigned char* data, int width, int height, unsigned int format);
    };
}