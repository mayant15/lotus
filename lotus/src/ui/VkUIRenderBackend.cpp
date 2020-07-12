#include <iostream>
#include "lotus/ui/UIRenderBackend.h"

VkUIRenderBackend::VkUIRenderBackend()
{
    std::cout << "Please don't use the Vulkan backend :)" << std::endl;
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
