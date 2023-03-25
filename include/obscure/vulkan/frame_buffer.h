#ifndef OBSCURE_VULKAN_FRAME_BUFFER_DEFINITION
#define OBSCURE_VULKAN_FRAME_BUFFER_DEFINITION 1
#include "obscure/vulkan/image_view.h"
#include "obscure/vulkan/render_pass.h"

namespace obscure
{
	namespace vulkan
	{
		struct frame_buffer
		{
			VkFramebuffer vk_frame_buffer;
			frame_buffer() noexcept;
			frame_buffer(device device,image_view view, render_pass pass, VkExtent2D extent, VkAllocationCallbacks const* allocator = nullptr);

			VkFramebuffer get_handle() const noexcept;
			void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif