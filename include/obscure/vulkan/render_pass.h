#ifndef OBSCURE_VULKAN_RENDER_PASS_DEFINITION
#define OBSCURE_VULKAN_RENDER_PASS_DEFINITION 1
#include "obscure/vulkan/device.h"
namespace obscure
{
	namespace vulkan
	{
		struct render_pass
		{
			VkRenderPass vk_render_pass;
			render_pass() noexcept;
			render_pass(device _device, VkRenderPassCreateInfo create_info, VkAllocationCallbacks const* allocator = nullptr);

			VkRenderPass get_handle() const noexcept;

			void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}

#endif