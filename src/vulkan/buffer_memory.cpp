#include "obscure/vulkan/buffer_memory.h"

obscure::vulkan::buffer_memory::buffer_memory(vulkan::device _device, vulkan::device_memory _memory, size_t size)
	: device(_device), device_memory(_memory)
{
	size_t actual = (device_memory.memory_size < size) ? device_memory.memory_size : size;
	vkMapMemory(device.get_handle(), device_memory.get_handle(), 0, actual, 0, &mapped_memory);
}
obscure::vulkan::buffer_memory::~buffer_memory()
{
	VkMappedMemoryRange range{};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;

	range.memory = device_memory.get_handle();
	range.offset = 0;
	range.size = VK_WHOLE_SIZE;

	vkFlushMappedMemoryRanges(device.get_handle(), 1, &range);
	vkUnmapMemory(device.get_handle(), device_memory.get_handle());
}

template<typename T>
T& obscure::vulkan::buffer_memory::at(size_t index)&
{
	return reinterpret_cast<T*>(mapped_memory)[index];
}

template<typename T>
T const& obscure::vulkan::buffer_memory::at(size_t index) const&
{
	return reinterpret_cast<T*>(mapped_memory)[index];
}