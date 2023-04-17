#include "obscure/vulkan/owning_buffer.h"

void free(obscure::vulkan::memory_owning_buffer* buffer) noexcept
{
	if (buffer->counter)
	{
		uint64_t buffer_count = buffer->counter->fetch_sub(1);
		//only this buffer remains
		if (buffer_count == 1)
		{
			vkDeviceWaitIdle(buffer->device.get_handle());
			vkDestroyBuffer(buffer->device.get_handle(), buffer->vk_buffer, buffer->allocator);
			buffer->memory.free(buffer->device, buffer->allocator);
			delete buffer->counter;
		}
	}
}

obscure::vulkan::memory_owning_buffer::memory_owning_buffer() noexcept
	: vk_buffer(VK_NULL_HANDLE), memory(), device(), allocator(nullptr), counter(nullptr)
{}


obscure::vulkan::memory_owning_buffer::memory_owning_buffer(vulkan::device _device, size_t size, VkBufferUsageFlags usage, VkPhysicalDeviceMemoryProperties properties, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* _allocator)
	: device(_device), allocator(_allocator), counter(new std::atomic_uint64_t(1))
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = nullptr;

	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(device.get_handle(), &bufferInfo, allocator, &vk_buffer);

	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(device.get_handle(), vk_buffer, &reqs);

	memory = obscure::vulkan::device_memory(device, properties, reqs, requested_properties);

	vkBindBufferMemory(device.get_handle(), vk_buffer, memory.get_handle(), 0);
}

obscure::vulkan::memory_owning_buffer::memory_owning_buffer(memory_owning_buffer const& other) noexcept
	: vk_buffer(other.vk_buffer), memory(other.memory), device(other.device), allocator(other.allocator), counter(other.counter)
{
	if (counter)
	{
		counter->fetch_add(1);
	}
}

obscure::vulkan::memory_owning_buffer& obscure::vulkan::memory_owning_buffer::operator=(memory_owning_buffer const& other) noexcept
{
	if (counter != other.counter)
	{
		free(this);
		new (this) memory_owning_buffer(other);
	}
	return *this;
}


VkBuffer obscure::vulkan::memory_owning_buffer::get_handle() const& noexcept
{
	return vk_buffer;
}





obscure::vulkan::memory_owning_buffer::~memory_owning_buffer()
{
	free(this);
}

obscure::vulkan::memory_owning_staging_buffer::memory_owning_staging_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator)
	: memory_owning_buffer(_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, properties, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, allocator)
{}

void obscure::vulkan::memory_owning_staging_buffer::write_data(const void* data, size_t size)&
{
	void* destination;
	size_t actual = (memory.memory_size < size) ? memory.memory_size : size;
	vkMapMemory(device.get_handle(), memory.get_handle(), 0, actual, 0, &destination);
	memcpy_s(destination, actual, data, actual);
	
	VkMappedMemoryRange range{};
	range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	range.pNext = nullptr;

	range.memory = memory.get_handle();
	range.offset = 0;
	range.size = VK_WHOLE_SIZE;

	vkFlushMappedMemoryRanges(device.get_handle(), 1, &range);
	vkUnmapMemory(device.get_handle(), memory.get_handle());
}

obscure::vulkan::buffer_memory obscure::vulkan::memory_owning_staging_buffer::map_memory(size_t size)&
{
	return buffer_memory(device, memory, size);
}

obscure::vulkan::memory_owning_vertex_buffer::memory_owning_vertex_buffer(vulkan::device _device, size_t buffer_size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator)
	: memory_owning_buffer(_device, buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, properties, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, allocator)
{}