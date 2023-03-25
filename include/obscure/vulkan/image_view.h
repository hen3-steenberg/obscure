#ifndef OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION
#define OBSCURE_VULKAN_IMAGE_VIEW_DEFINITION 1
#include "obscure/vulkan/device.h"
namespace obscure
{
	namespace vulkan
	{
		struct image_view
		{
			VkImageView vk_image_view;
			image_view() noexcept;
			image_view(device _device, VkImage image, VkFormat image_format, VkAllocationCallbacks const* allocator = nullptr);
			VkImageView get_handle() const noexcept;
			void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif