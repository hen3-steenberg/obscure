#ifndef OBSCURE_VULKAN_FENCE_DEFINITION
#define OBSCURE_VULKAN_FENCE_DEFINITION 1
#include "obscure/vulkan/device.h"
#include <array>
namespace obscure
{
	namespace vulkan
	{
		struct fence
		{
			VkFence vk_fence;
			VkDevice vk_device;

			fence() noexcept;
			fence(device device, bool state, VkAllocationCallbacks const* allocator = nullptr);

			VkFence get_handle() const noexcept;

			void wait();
			void reset();
			void free(VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}

#endif