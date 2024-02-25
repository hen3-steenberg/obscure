module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.semaphore;

import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct semaphore
			{
				VkSemaphore vk_semaphore;
				semaphore() noexcept;
				semaphore(device device, VkAllocationCallbacks const* allocator = nullptr);
				VkSemaphore get_handle() const noexcept;
				VkSemaphore const* get_handle_address() const noexcept;
				void free(device device, VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}