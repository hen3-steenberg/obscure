#ifndef OBSCURE_VULKAN_SEMAPHORE_DEFINITION
#define OBSCURE_VULKAN_SEMAPHORE_DEFINITION 1
#include "obscure/vulkan/device.h"
namespace obscure
{
	namespace vulkan
	{
		struct semaphore
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
#endif