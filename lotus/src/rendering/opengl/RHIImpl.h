#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <rendering/RHI.h>
#include <lotus/rendering/common.h>
#include <lotus/debug.h>

#include <unordered_map>

/**
 * OpenGL implementation of the render hardware interface. All functions here
 * MUST HAVE the GL prefix, and have names exactly as defined in RHI.h
 *
 * For example, RHI::CreateTexture() becomes RHI::Detail::GLCreateTexture()
 *
 * This is because implementations in RHI.cpp are generated with a macro for convenience.
 */

namespace Lotus::RHI::Detail
{
    static const std::unordered_map<GLuint, std::string> messages = {
            // Source
            {GL_DEBUG_SOURCE_API, "Source: API"},
            {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Source: Window System"},
            {GL_DEBUG_SOURCE_SHADER_COMPILER, "Source: Shader Compiler"},
            {GL_DEBUG_SOURCE_THIRD_PARTY, "Source: Third Party"},
            {GL_DEBUG_SOURCE_APPLICATION, "Source: Application"},
            {GL_DEBUG_SOURCE_OTHER, "Source: Other"},

            // Type
            {GL_DEBUG_TYPE_ERROR, "Type: Error"},
            {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Type: Deprecated Behaviour"},
            {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Type: Undefined Behaviour"},
            {GL_DEBUG_TYPE_PORTABILITY, "Type: Portability"},
            {GL_DEBUG_TYPE_PERFORMANCE, "Type: Performance"},
            {GL_DEBUG_TYPE_MARKER, "Type: Marker"},
            {GL_DEBUG_TYPE_PUSH_GROUP, "Type: Push Group"},
            {GL_DEBUG_TYPE_POP_GROUP, "Type: Pop Group"},
            {GL_DEBUG_TYPE_OTHER, "Type: Other"},
    };

    GLuint GLStorageFormatFromEnum(EStorageFormat format)
    {
        switch (format)
        {
            case EStorageFormat::RED:
                return GL_RED;
            case EStorageFormat::RGB:
                return GL_RGB;
            case EStorageFormat::RGBA:
                return GL_RGBA;
            case EStorageFormat::RGB16F:
                return GL_RGB16F;
            case EStorageFormat::DEPTH_COMPONENT24:
                return GL_DEPTH_COMPONENT24;
            case EStorageFormat::DEPTH_COMPONENT:
                return GL_DEPTH_COMPONENT;
        }
    }

    GLuint GLDataTypeFromEnum(ETextureDataType dataType)
    {
        switch (dataType)
        {
            case ETextureDataType::FLOAT:
                return GL_FLOAT;
            case ETextureDataType::UNSIGNED_BYTE:
                return GL_UNSIGNED_BYTE;
        }
    }

    void GLPlatformInit()
    {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            throw std::exception("Failed to initialize GLAD");
        }
    }

    void GLSetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    TextureID GLCreateTexture(const TextureInfo& info)
    {
        TextureID id;
        glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GLStorageFormatFromEnum(info.InternalFormat), info.Width, info.Height, 0,
                     GLStorageFormatFromEnum(info.Format), GLDataTypeFromEnum(info.DataType), info.Data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return id;
    }

    CubeMap GLCreateCubeMap(const TextureInfo& info)
    {
        TextureID id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GLStorageFormatFromEnum(info.InternalFormat),
                         info.Width, info.Height, 0,
                         GLStorageFormatFromEnum(info.Format), GL_FLOAT, info.Data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return id;
    }

    VAO GLCreateVAO(const VAOInfo& info)
    {
        VAO vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        VBO vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, info.Size, info.Vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        return vao;
    }

    inline void GLBindVAO(VAO vao)
    {
        glBindVertexArray(vao);
    }


    inline void GLDrawElements(unsigned int size, const void* indices)
    {
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, indices);
    }

    inline void GLDrawTriangles(unsigned int count)
    {
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    inline void GLSetCullFace(unsigned int face)
    {
        if (face == RHI::CULL_FACE_BACK) glCullFace(GL_BACK);
        else if (face == RHI::CULL_FACE_FRONT) glCullFace(GL_FRONT);
        else glCullFace(GL_BACK);
    }

    inline void GLSetActiveTextureSlot(TextureSlot slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    inline void GLBindTexture(TextureID id)
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    inline void GLBindCubeMap(TextureID id)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    inline void GLSetViewport(unsigned int width, unsigned int height)
    {
        glViewport(0, 0, width, height);
    }

    inline void GLBindFrameBuffer(FrameBuffer fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    // NOTE: Attaches a 512x512 renderbuffer object by default
    inline FrameBuffer GLCreateFrameBuffer(const FrameBufferInfo& info)
    {
        FrameBuffer fb;
        glGenFramebuffers(1, &fb);

        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        if (!info.HasColorBuffer)
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return fb;
    }

    inline void GLAttachRenderBuffer(FrameBuffer fb, const RenderBufferAttachmentInfo& info)
    {
        BindFrameBuffer(fb);

        RenderBuffer rb;
        glGenRenderbuffers(1, &rb);
        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorage(GL_RENDERBUFFER, GLStorageFormatFromEnum(info.InternalFomat), info.Width,
                              info.Height);

        switch (info.Type)
        {
            case EAttachmentType::COLOR:
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb);
                break;
            case EAttachmentType::DEPTH:
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);
                break;
            case EAttachmentType::STENCIL:
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb);
                break;
        }

        BindFrameBuffer(0);
    }

    inline void GLAttachTexture(FrameBuffer fb, const TextureAttachmentInfo& info)
    {
        switch (info.Type)
        {
            case EAttachmentType::COLOR:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, info.TextureTarget, info.ID, 0);
                break;
            case EAttachmentType::DEPTH:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, info.TextureTarget, info.ID, 0);
                break;
            case EAttachmentType::STENCIL:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, info.TextureTarget, info.ID, 0);
                break;
        }

    }

    inline void GLSetDepthFunction(unsigned int func)
    {
        if (func == RHI::DEPTH_FUNC_LEQUAL) glDepthFunc(GL_LEQUAL);
        else glDepthFunc(GL_LESS);
    }

    inline unsigned int GLGetCubeMapPositiveX()
    {
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    }

    void GLClear(unsigned int bits)
    {
        unsigned int glbits = 0;
        if (bits & COLOR_BIT) glbits |= GL_COLOR_BUFFER_BIT;
        if (bits & DEPTH_BIT) glbits |= GL_DEPTH_BUFFER_BIT;

        glClear(glbits);
    }

    inline unsigned int GLGetTexture2DTarget()
    {
        return GL_TEXTURE_2D;
    }

    void GLEnableFeatures(unsigned int bits)
    {
        if (bits & ENABLE_DEPTH_TEST) glEnable(GL_DEPTH_TEST);
        if (bits & ENABLE_CULL_FACE) glEnable(GL_CULL_FACE);
        if (bits & ENABLE_MSAA) glEnable(GL_MULTISAMPLE);
    }

    void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;
        std::string sourceString = messages.at(source);
        std::string typeString = messages.at(type);
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
            case GL_DEBUG_SEVERITY_LOW:
                LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
            default:
                LOG_INFO  ("({}): {}. {}. {}", id, message, sourceString, typeString);
                break;
        }
    }

    inline void GLEnableDebugContext()
    {
        LOG_INFO("Setting up a debug context...");
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugMessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    void GLCreateBuffersForMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, unsigned int* vao, unsigned int* vbo, unsigned int* ebo)
    {
        glGenVertexArrays(1, vao);
        glGenBuffers(1, vbo);
        glGenBuffers(1, ebo);

        glBindVertexArray(*vao);
        glBindBuffer(GL_ARRAY_BUFFER, *vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Can do this because structs are laid sequentially in memory

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, TexCoords));

        // vertex tangents
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, Tangent));

        glBindVertexArray(0);
    }
}
