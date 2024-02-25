module;
#include <cstdint>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.device_memory;
import obscure.internal.vulkan.device;
namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct device_memory
			{
				VkDeviceMemory vk_memory;
				size_t memory_size;
				device_memory() noexcept;
				device_memory(device device, size_t size, uint32_t memory_index, const VkAllocationCallbacks* allocator = nullptr);
				VkDeviceMemory get_handle() const noexcept;
				void free(device device, const VkAllocationCallbacks* allocator = nullptr) noexcept;

				static uint32_t find_memory_index(VkPhysicalDeviceMemoryProperties properties, VkMemoryRequirements requirements, VkMemoryPropertyFlags requested_properties);
			};

			
		}
	}
}