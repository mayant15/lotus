#include "lotus/resources/Cubemap.h"
#include "lotus/resources/AssetRegistry.h"
#include "lotus/debug.h"
#include "lotus/physics.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace Lotus
{
    float SKYBOX_VERTICES[] = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

    SRef<Cubemap> CubemapLoader::Load(const std::vector<std::string>& faces) const
    {
        SRef<Cubemap> map = std::make_shared<Cubemap>();

        glGenVertexArrays(1, &map->VAO);
        glGenBuffers(1, &map->VBO);

        glBindVertexArray(map->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, map->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), &SKYBOX_VERTICES, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        stbi_set_flip_vertically_on_load(false);
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                             data);
                stbi_image_free(data);
            }
            else
            {
                LOG_ERROR("Cubemap texture failed to load at path: {}", faces[i]);
                stbi_image_free(data);
                throw std::invalid_argument("Cubemap load failed.");
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        map->ID = textureID;
        return map;
    }

    void captureCubemap(unsigned int flatmap, unsigned int cubemap, unsigned int FBO, unsigned int VAO)
    {
        Matrix4f captureProjection = LPerspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        Matrix4f captureViews[] = 
        {
            LLookAt(ORIGIN,         X_AXIS, -1.0f * Y_AXIS),
            LLookAt(ORIGIN, -1.0f * X_AXIS, -1.0f * Y_AXIS),
            LLookAt(ORIGIN,         Y_AXIS,         Z_AXIS),
            LLookAt(ORIGIN, -1.0f * Y_AXIS, -1.0f * Z_AXIS),
            LLookAt(ORIGIN,         Z_AXIS, -1.0f * Y_AXIS),
            LLookAt(ORIGIN, -1.0f * Z_AXIS, -1.0f * Y_AXIS),

           // glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           // glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
           // glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
           // glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           // glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // convert HDR equirectangular environment map to cubemap equivalent
        auto shader= GET(AssetRegistry).LoadShader(INTERNAL_SHADERS("loadHDRI"));

        shader->Use();
        shader->SetInt("hdri", 0);
        shader->SetMat4f("projection", captureProjection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, flatmap);

        // Configure the viewport to the capture dimensions
        glViewport(0, 0, 512, 512);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            shader->SetMat4f("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // Reference: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
    SRef<Cubemap> CubemapLoader::Load(const std::string& hdri) const
    {
        SRef<Cubemap> map = std::make_shared<Cubemap>();

        glGenVertexArrays(1, &map->VAO);
        glGenBuffers(1, &map->VBO);

        glBindVertexArray(map->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, map->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), &SKYBOX_VERTICES, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        unsigned int captureFBO, captureRBO;
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        unsigned int cubemap;
        glGenTextures(1, &cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            // note that we store each face with 16 bit floating point values
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                         512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned int flatmap;
        stbi_set_flip_vertically_on_load(true);
        int width, height, nChannels;
        float* data = stbi_loadf(hdri.c_str(), &width, &height, &nChannels, 0);
        if (data)
        {
            glGenTextures(1, &flatmap);
            glBindTexture(GL_TEXTURE_2D, flatmap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
        
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            captureCubemap(flatmap, cubemap, captureFBO, map->VAO);

            stbi_image_free(data);
        }
        else
        {
            LOG_ERROR("HDRI failed to load at path: {}", hdri);
            stbi_image_free(data);
            throw std::invalid_argument("Cubemap load failed.");
        }

        map->ID = cubemap;
        return map;
    }
}
