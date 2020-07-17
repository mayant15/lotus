#include <lotus/debug.h>
#include "lotus/ui/UIRenderBackend.h"

VkUIRenderBackend::VkUIRenderBackend()
{
    LOG_WARN("Please don't use the Vulkan backend :)");
}

void VkUIRenderBackend::shutdown()
{
    //
}

void VkUIRenderBackend::newFrame()
{
    //
}

void VkUIRenderBackend::renderDrawData(ImDrawData* drawData)
{
    //
}
