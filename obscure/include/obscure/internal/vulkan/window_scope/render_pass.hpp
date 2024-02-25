module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.render_pass;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct render_pass
			{
				VkRenderPass vk_render_pass;
				render_pass() noexcept;
				render_pass(device device, VkFormat format, VkAllocationCallbacks const* allocator = nullptr);

				VkRenderPass get_handle() const noexcept;

				void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}