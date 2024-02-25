module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.pipeline_layout;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct pipeline_layout
			{
				VkPipelineLayout vk_layout;

				pipeline_layout() noexcept;
				pipeline_layout(device _device, VkPipelineLayoutCreateInfo create_info, VkAllocationCallbacks const* allocator = nullptr);


				VkPipelineLayout get_handle() const noexcept;

				void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}