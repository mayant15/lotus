#pragma once

#include <lotus/lcommon.h>
#include <string>
#include <filesystem>

namespace Lotus
{
    struct ProjectConfig
    {
        std::filesystem::path ProjectResourceRoot;
        std::string StartScene;
    };
    SERIALIZE(ProjectConfig, StartScene);

    struct BuildConfig
    {
        bool IsDebug;
    };
    SERIALIZE(BuildConfig, IsDebug);

    /** @brief Supported Render APIs. */
    enum class ERenderAPI
    {
        OPEN_GL,
        DIRECTX,
        VULKAN
    };
    SERIALIZE_ENUM(ERenderAPI, {
        {ERenderAPI::OPEN_GL, "opengl"},
        {ERenderAPI::DIRECTX, "directx"},
        {ERenderAPI::VULKAN, "vulkan"}
    });

    /** @brief Options to initialize the renderer with. */
    struct RenderConfig
    {
        ERenderAPI RenderAPI;
        unsigned int ViewportWidth;
        unsigned int ViewportHeight;

        bool DepthTest;
        bool CullFace;
        bool MSAA;
    };
    SERIALIZE(RenderConfig, RenderAPI, ViewportWidth, ViewportHeight, DepthTest, CullFace, MSAA);

    LOTUS_API void LoadConfig(const std::string& path);
    LOTUS_API const ProjectConfig& GetProjectConfig();
    LOTUS_API const RenderConfig& GetRenderConfig();
    LOTUS_API const BuildConfig& GetBuildConfig();
}
