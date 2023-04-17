#ifndef OBSCURE_VULKAN_DEVICE_DEFINITION
#define OBSCURE_VULKAN_DEVICE_DEFINITION 1

#include "obscure/vulkan/surface.h"

namespace obscure
{
	namespace vulkan
	{
		struct device
		{
			VkDevice vk_device;
			uint32_t graphics_queue_index;
			uint32_t present_queue_index;
			uint32_t transfer_queue_index;
			device() noexcept;
			device(VkPhysicalDevice device, surface surface, VkAllocationCallbacks const* allocator = nullptr);
			VkDevice get_handle() const noexcept;

			VkQueue get_graphics_queue() const;

			VkQueue get_present_queue() const;

			VkQueue get_transfer_queue() const;

			void wait_for_idle();

			void free(VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}
#endif