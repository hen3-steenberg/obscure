module;
#include <span>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.command_pool;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct command_pool
			{
				VkCommandPool vk_command_pool;
				command_pool() noexcept;

				command_pool(device device, uint32_t queue_family_index, VkCommandPoolCreateFlags flags, VkAllocationCallbacks const* allocator = nullptr);
				VkCommandPool get_handle() const noexcept;
				void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
				VkCommandBuffer allocate_primary_command_buffer(device device) const;
				void allocate_primary_command_buffers(device device, std::span<VkCommandBuffer> command_buffers) const;

				static command_pool create_graphics_pool(device device, VkAllocationCallbacks const* allocator = nullptr);

				static command_pool create_transfer_pool(device device, VkAllocationCallbacks const* allocator = nullptr);

			};
		}
	}
}