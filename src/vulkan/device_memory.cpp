#include "obscure/vulkan/device_memory.h"
#include <system_error>

uint32_t get_memory_type_index(VkPhysicalDeviceMemoryProperties properties, VkMemoryRequirements requirements, VkMemoryPropertyFlags requested_properties)
{
	for (uint32_t index = 0; index < properties.memoryTypeCount; ++index)
	{
		if (
			(requirements.memoryTypeBits & (1 << index)) && 
			((properties.memoryTypes[index].propertyFlags & requested_properties) == requested_properties)
		)
		{
			return index;
		}
	}
	throw std::errc::result_out_of_range;
}

obscure::vulkan::device_memory::device_memory() noexcept
	: vk_memory(VK_NULL_HANDLE), memory_size(0)
{}

obscure::vulkan::device_memory::device_memory(device device, VkPhysicalDeviceMemoryProperties properties, VkMemoryRequirements requirements, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* allocator)
	: memory_size(requirements.size)
{
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.allocationSize = memory_size;
	allocInfo.memoryTypeIndex = get_memory_type_index(properties, requirements, requested_properties);

	vkAllocateMemory(device.get_handle(), &allocInfo, allocator, &vk_memory);
}


VkDeviceMemory obscure::vulkan::device_memory::get_handle() const noexcept
{
	return vk_memory;
}

void obscure::vulkan::device_memory::free(device device, const VkAllocationCallbacks* allocator) noexcept
{
	vkFreeMemory(device.get_handle(), vk_memory, allocator);
}