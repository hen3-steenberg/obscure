#ifndef OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION
#define OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION 1
#include "device.h"
#include "reference.hpp"
namespace obscure
{
	namespace vulkan
	{
		struct image_view : reference<VkImageView>
		{
			image_view(device _device, VkImage image, VkFormat image_format);
		};
	}
}
#endif