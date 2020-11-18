
#include "Common.h"

VkImageView HelloTriangleApplication::createTextureImageView(VkImage vkImg)
{
    // createImageView from create image views
    return createImageView(vkImg, VK_FORMAT_R8G8B8A8_SRGB,
        VK_IMAGE_ASPECT_COLOR_BIT);
}
