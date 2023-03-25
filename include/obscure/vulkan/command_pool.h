#ifndef OBSCURE_VULKAN_COMMAND_POOL_DEFINITION
#define OBSCURE_VULKAN_COMMAND_POOL_DEFINITION 1
#include "obscure/vulkan/device.h"
namespace obscure
{
	namespace vulkan
	{
		struct command_pool
		{
			VkCommandPool vk_command_pool;
			command_pool() noexcept;

			command_pool(device device,uint32_t queue_family_index, VkAllocationCallbacks const* allocator = nullptr);
			VkCommandPool get_handle() const noexcept;
			void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			VkCommandBuffer allocate_primary_command_buffer(device device) const;

		};
	}
}
#endif