module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.device;

import obscure.internal.vulkan.surface;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			struct swap_chain;

			export struct device
			{
			private:
				VkDevice vk_device;

				
			public:
				VkPhysicalDevice physical_device;
				uint32_t graphics_queue_index;
				uint32_t present_queue_index;
				uint32_t transfer_queue_index;

				device() noexcept;
				device(VkPhysicalDevice device, obscure::internal::vulkan::surface surface, VkAllocationCallbacks const* allocator = nullptr);

				VkDevice get_handle() const& noexcept;

				VkQueue get_graphics_queue() const&;

				VkQueue get_present_queue() const&;

				VkQueue get_transfer_queue() const&;

				VkPhysicalDeviceMemoryProperties get_memory_properties() const&;

				void wait_for_idle() &;

				void free(VkAllocationCallbacks const* allocator = nullptr) & noexcept;
			};
		}
	}
}