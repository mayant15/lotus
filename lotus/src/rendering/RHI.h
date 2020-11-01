#pragma once

#include "lotus/debug.h"

#include <glad/glad.h>

#include <vector>
#include <exception>

// TODO: Delegate calls to the active platform's API
namespace Lotus::RHI
{
    typedef unsigned int FrameBuffer;
    typedef unsigned int RenderBuffer;

    typedef unsigned int VAO;
    typedef unsigned int VBO;

    typedef unsigned int TextureID;
    typedef unsigned int CubeMap;
    // TODO: Formalize texture slots. Have a specification for the purpose of each slot
    typedef unsigned int TextureSlot;


    struct FrameBufferInfo
    {
        //
    };

    enum class EAttachmentType
    {
        COLOR,
        DEPTH,
        STENCIL
    };

    struct RenderBufferAttachmentInfo
    {
        EAttachmentType Type;
        unsigned int InternalFomat;
        unsigned int Width;
        unsigned int Height;
    };

    struct TextureAttachmentInfo
    {
        EAttachmentType Type;
        TextureID ID;
        unsigned int TextureTarget;
    };

    /**
     * @brief Format of the texture data
     *
     * This enum wraps API specific pixel data formats: RED, RGB, RGBA
     */
    enum class ETextureFormat
    {
        RED = GL_RED,
        RGB = GL_RGB,
        RGBA = GL_RGBA,
        RGB16F = GL_RGB16F
    };

    /**
     * @brief Return a data format corresponding to the number of color channels
     * @param nChannels Number of color channels
     * @return ETextureFormat Representing the channels
     * @throws std::runtime_error If the number of channels has no corresponding data format
     */
    inline ETextureFormat FormatFromChannel(unsigned int nChannels)
    {
        switch (nChannels)
        {
            case 1: return ETextureFormat::RED;
            case 3: return ETextureFormat::RGB;
            case 4: return ETextureFormat::RGBA;
            default:
            {
                LOG_ERROR("Invalid Color Format");
                throw std::runtime_error("Invalid color format");
            }
        }
    }

    /** @brief Configuration options to pass when creating textures */
    struct TextureInfo
    {
        // TODO: Mipmap and wrapping options
        // TODO: Formalize DataType

        /** @brief Pointer to the pixel data that is to be set for the texture */
        void* Data = nullptr;

        /** @brief Width of the texture */
        unsigned int Width = 512;

        /** @brief Height of the texture */
        unsigned int Height = 512;

        /** @brief Format for the pixel data */
        ETextureFormat Format = ETextureFormat::RGB;

        /** @brief Internal format for storing the texture. Usually the same as Format */
        ETextureFormat InternalFormat = ETextureFormat::RGB;

        /** @brief Data type of the pixel data */
        unsigned int DataType = GL_UNSIGNED_BYTE;
    };

    /** @brief Configuration options to pass when creating a Vertex Array Object */
    struct VAOInfo
    {
        /** @brief Vertices to be placed in the corresponding VBO */
        float* Vertices;

        /** @brief Size of the Vertices array in bytes */
        unsigned int Size;
    };

    /**
     * @brief Create a texture
     * @param info Options
     * @return Identifier for the created texture
     */
    TextureID CreateTexture(const TextureInfo& info);

    /**
     * @brief Create a cube map texture
     * @param info Options
     * @return Identifier for the created cube map
     */
    CubeMap CreateCubeMap(const TextureInfo& info);

    /**
     * @brief Set the active texture slot
     * @param slot TextureSlot to activate
     */
    void SetActiveTextureSlot(TextureSlot slot);

    /**
     * @brief Bind a texture to the active slot
     * @param id Identifier for the texture to bind
     */
    void BindTexture(TextureID id);

    /**
     * @brief Bind a cubemap to the active slot
     * @param id Identifier for the texture to bind
     */
    void BindCubeMap(TextureID id);

    /**
     * @brief Create a frame buffer
     * @param info Options
     * @return Identifier for the created frame buffer
     */
    FrameBuffer CreateFrameBuffer(const FrameBufferInfo& info);

    // TODO: Attach renderbuffer then choose depth/color or vice versa?
    void AttachRenderBuffer(FrameBuffer fb, const RenderBufferAttachmentInfo& info);
    void AttachTexture(FrameBuffer fb, const TextureAttachmentInfo& info);

    /**
     * @brief Bind a frame buffer
     * @param fbo Identifier for the frame buffer to bind
     */
    void BindFrameBuffer(FrameBuffer fbo);

    /**
     * @brief Create a vertex array object with an associated vertex buffer object
     * @param info Options
     * @return Identifier for the created VAO
     */
    VAO CreateVAO(const VAOInfo& info);

    /**
     * @brief Bind a vertex array object
     * @param fbo Identifier for the VAO to bind
     */
    void BindVAO(VAO vao);

    /**
     * @brief Set the render viewport dimensions
     * @param width Desired width of the viewport
     * @param height Desired height of the viewport
     */
    void SetViewport(unsigned int width, unsigned int height);

    /**
     * @brief Draw triangles defined in the currently bound VAO
     * @param count Number of triangles to draw
     */
    void DrawTriangles(unsigned int count);
}
