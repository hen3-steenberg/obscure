module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.image_view;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct image_view
			{
				VkImageView vk_image_view;
				image_view() noexcept;
				image_view(device _device, VkImage image, VkFormat image_format, VkAllocationCallbacks const* allocator = nullptr);
				VkImageView get_handle() const noexcept;
				VkImageView const* get_handle_address() const noexcept;
				void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}