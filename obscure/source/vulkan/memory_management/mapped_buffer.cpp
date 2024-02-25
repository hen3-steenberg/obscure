module;
#include <cstring>
#include "glfw_vulkan_include.h"
module obscure.vulkan.mapped_buffer;

obscure::vulkan::mapped_buffer::mapped_buffer() noexcept
	: buffer(), device(), mapped_memory(nullptr), allocator(nullptr), buffer_size(0)
{}

obscure::vulkan::mapped_buffer::mapped_buffer(obscure::internal::vulkan::device _device, size_t _size, VkBufferUsageFlags buffer_usage, const VkAllocationCallbacks* _allocator)
	: buffer(_device, _size, buffer_usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, _allocator), 
	device(_device), mapped_memory(nullptr), allocator(_allocator), buffer_size(_size)
{
	vkMapMemory(device.get_handle(), buffer.get_memory_handle(), 0, buffer_size, 0, &mapped_memory);
}

obscure::vulkan::mapped_buffer::mapped_buffer(mapped_buffer const& other, VkBufferUsageFlags buffer_usage)
	: buffer(other.device, other.buffer_size, buffer_usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, other.allocator),
	device(other.device), mapped_memory(nullptr), allocator(other.allocator), buffer_size(other.buffer_size)
{
	vkMapMemory(device.get_handle(), buffer.get_memory_handle(), 0, buffer_size, 0, &mapped_memory);
	std::memcpy(mapped_memory, other.mapped_memory, buffer_size);
}

obscure::vulkan::mapped_buffer::mapped_buffer(mapped_buffer&& other) noexcept
	: buffer(other.buffer), device(other.device), mapped_memory(other.mapped_memory),
	allocator(other.allocator), buffer_size(other.buffer_size)
{
	other.mapped_memory = nullptr;
}

void obscure::vulkan::mapped_buffer::write_data()&
{
	VkMappedMemoryRange memory_range{};
	memory_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memory_range.pNext = nullptr;

	memory_range.memory = buffer.get_memory_handle();
	memory_range.offset = 0;
	memory_range.size = buffer_size;

	vkFlushMappedMemoryRanges(device.get_handle(), 1, &memory_range);
}

void obscure::vulkan::mapped_buffer::read_data()&
{
	VkMappedMemoryRange memory_range{};
	memory_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memory_range.pNext = nullptr;

	memory_range.memory = buffer.get_memory_handle();
	memory_range.offset = 0;
	memory_range.size = buffer_size;

	vkInvalidateMappedMemoryRanges(device.get_handle(), 1, &memory_range);
}

obscure::vulkan::mapped_buffer::~mapped_buffer()
{
	if (mapped_memory)
	{
		vkUnmapMemory(device.get_handle(), buffer.get_memory_handle());
		buffer.free(device, allocator);
	}
}