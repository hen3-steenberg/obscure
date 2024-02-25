module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.fence;

import obscure.internal.vulkan.device;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct fence
			{
				VkFence vk_fence;
				VkDevice vk_device;

				fence() noexcept;
				fence(device device, bool signaled, VkAllocationCallbacks const* allocator = nullptr);

				VkFence get_handle() const noexcept;

				void wait();
				void reset();
				void free(VkAllocationCallbacks const* allocator = nullptr) noexcept;
			};
		}
	}
}