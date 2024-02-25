module;
#include <system_error>
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.device_memory;

obscure::internal::vulkan::device_memory::device_memory() noexcept
	: vk_memory(VK_NULL_HANDLE), memory_size(0)
{}

obscure::internal::vulkan::device_memory::device_memory(device device, size_t size, uint32_t memory_index, const VkAllocationCallbacks* allocator)
	: memory_size(size)
{
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;

	allocInfo.allocationSize = memory_size;
	allocInfo.memoryTypeIndex = memory_index;

	vkAllocateMemory(device.get_handle(), &allocInfo, allocator, &vk_memory);
}


VkDeviceMemory obscure::internal::vulkan::device_memory::get_handle() const noexcept
{
	return vk_memory;
}

void obscure::internal::vulkan::device_memory::free(device device, const VkAllocationCallbacks* allocator) noexcept
{
	vkFreeMemory(device.get_handle(), vk_memory, allocator);
}

uint32_t obscure::internal::vulkan::device_memory::find_memory_index(VkPhysicalDeviceMemoryProperties properties, VkMemoryRequirements requirements, VkMemoryPropertyFlags requested_properties)
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