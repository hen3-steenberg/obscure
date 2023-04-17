#ifndef OBSCURE_VULKAN_DEVICE_MEMORY_DEFINITION
#define OBSCURE_VULKAN_DEVICE_MEMORY_DEFINITION 1

#include "obscure/vulkan/device.h"

namespace obscure
{
	namespace vulkan
	{
		struct device_memory
		{
			VkDeviceMemory vk_memory;
			size_t memory_size;
			device_memory() noexcept;
			device_memory(device device, VkPhysicalDeviceMemoryProperties properties, VkMemoryRequirements requirements, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* allocator = nullptr);
			VkDeviceMemory get_handle() const noexcept;
			void free(device device, const VkAllocationCallbacks* allocator = nullptr) noexcept;
		};
	}
}
#endif