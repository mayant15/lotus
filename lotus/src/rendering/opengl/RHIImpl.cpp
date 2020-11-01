#include "rendering/RHI.h"

namespace Lotus::RHI
{
    VAO CreateVAO(const VAOInfo& info)
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

    void BindVAO(VAO vao)
    {
        glBindVertexArray(vao);
    }

    void DrawTriangles(unsigned int count)
    {
        glDrawArrays(GL_TRIANGLES, 0, count);
    }

    void SetActiveTextureSlot(TextureSlot slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void BindTexture(TextureID id)
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void BindCubeMap(TextureID id)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void SetViewport(unsigned int width, unsigned int height)
    {
        glViewport(0, 0, width, height);
    }

    void BindFrameBuffer(FrameBuffer fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    CubeMap CreateCubeMap(const TextureInfo& info)
    {
        TextureID id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast<unsigned int>(info.InternalFormat),
                         info.Width, info.Height, 0,
                         static_cast<unsigned int>(info.Format), GL_FLOAT, info.Data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return id;
    }


    // NOTE: Attaches a 512x512 renderbuffer object by default
    FrameBuffer CreateFrameBuffer(const FrameBufferInfo& info)
    {
        FrameBuffer fb;
        RenderBuffer rb;
        glGenFramebuffers(1, &fb);
        return fb;
    }

    void AttachRenderBuffer(FrameBuffer fb, const RenderBufferAttachmentInfo& info)
    {
        BindFrameBuffer(fb);

        RenderBuffer rb;
        glGenRenderbuffers(1, &rb);
        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorage(GL_RENDERBUFFER, info.InternalFomat, info.Width, info.Height);

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

    void AttachTexture(FrameBuffer fb, const TextureAttachmentInfo& info)
    {
        switch (info.Type)
        {
            case EAttachmentType::COLOR:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, info.TextureTarget, info.ID, 0);
                break;
            case EAttachmentType::DEPTH:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT24, info.TextureTarget, info.ID, 0);
                break;
            case EAttachmentType::STENCIL:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, info.TextureTarget, info.ID, 0);
                break;
        }

    }

    TextureID CreateTexture(const TextureInfo& info)
    {
        TextureID id;
        glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<unsigned int>(info.InternalFormat), info.Width, info.Height, 0,
                     static_cast<unsigned int>(info.Format), info.DataType, info.Data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return id;
    }
}
