module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.frame_buffer;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.render_pass;
import obscure.internal.vulkan.image_view;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct frame_buffer
			{
				VkFramebuffer vk_frame_buffer;
				frame_buffer() noexcept;
				frame_buffer(device device, image_view view, render_pass pass, VkExtent2D extent, VkAllocationCallbacks const* allocator = nullptr);

				VkFramebuffer get_handle() const noexcept;
				void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}