#pragma once

#include <imgui.h>

class UIRenderBackend
{
public:
    virtual void shutdown() = 0;

    virtual void newFrame() = 0;

    virtual void renderDrawData(ImDrawData* drawData) = 0;
};

class GLUIRenderBackend : public UIRenderBackend
{
public:
    GLUIRenderBackend();

    void shutdown() override;

    void newFrame() override;

    void renderDrawData(ImDrawData* drawData) override;

private:
    unsigned int glVersion;
};

class VkUIRenderBackend : public UIRenderBackend
{
public:
    VkUIRenderBackend();

    void shutdown() override;

    void newFrame() override;

    void renderDrawData(ImDrawData* drawData) override;
};
