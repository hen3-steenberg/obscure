#ifndef OBSCURE_VULKAN_PIPELINE_LAYOUT_DEFINITION
#define OBSCURE_VULKAN_PIPELINE_LAYOUT_DEFINITION 1

#include "obscure/vulkan/device.h"

namespace obscure
{
	namespace vulkan
	{
		struct pipeline_layout
		{
			VkPipelineLayout vk_layout;

			pipeline_layout() noexcept;
			pipeline_layout(device _device, VkPipelineLayoutCreateInfo create_info, VkAllocationCallbacks const* allocator = nullptr);

			
			VkPipelineLayout get_handle() const noexcept;

			void free(device _device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif