module;
#include <cstring>
#include "glfw_vulkan_include.h"
module obscure.vulkan.staging_buffer;
import obscure.vulkan.data_transfer_batch;



obscure::vulkan::staging_buffer::staging_buffer() noexcept
	: host_buffer(), device_buffer(), device(), mapped_memory(nullptr), allocator(nullptr), buffer_size(0), transfer_pool()
{}

obscure::vulkan::staging_buffer::staging_buffer(obscure::internal::vulkan::device _device, obscure::internal::vulkan::command_pool transfer_cmd_pool, size_t _size, VkBufferUsageFlags buffer_usage, const VkAllocationCallbacks* _allocator)
	: host_buffer(_device, _size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, _allocator),
	device_buffer(_device, _size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | buffer_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _allocator),
	device(_device), allocator(_allocator), buffer_size(_size), transfer_pool(transfer_cmd_pool)
{
	vkMapMemory(device.get_handle(), host_buffer.get_memory_handle(), 0, buffer_size, 0, &mapped_memory);
}

obscure::vulkan::staging_buffer::staging_buffer(staging_buffer const& other, VkBufferUsageFlags buffer_usage)
	: host_buffer(other.device, other.buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT, other.allocator),
	device_buffer(other.device, other.buffer_size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | buffer_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, other.allocator),
	device(other.device), allocator(other.allocator), buffer_size(other.buffer_size), transfer_pool(other.transfer_pool)
{
	vkMapMemory(device.get_handle(), host_buffer.get_memory_handle(), 0, buffer_size, 0, &mapped_memory);
	std::memcpy(mapped_memory, other.mapped_memory, buffer_size);
}

obscure::vulkan::staging_buffer::staging_buffer(staging_buffer&& other) noexcept
	: host_buffer(other.host_buffer), device_buffer(other.device_buffer), device(other.device),
	allocator(other.allocator), buffer_size(other.buffer_size), transfer_pool(other.transfer_pool), mapped_memory(other.mapped_memory)
{
	other.mapped_memory = nullptr;
}

inline void obscure::vulkan::staging_buffer::sync_data()&
{
	VkMappedMemoryRange memory_range{};
	memory_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
	memory_range.pNext = nullptr;

	memory_range.memory = host_buffer.get_memory_handle();
	memory_range.offset = 0;
	memory_range.size = buffer_size;

	vkFlushMappedMemoryRanges(device.get_handle(), 1, &memory_range);
}

void obscure::vulkan::staging_buffer::write_data()&
{
	sync_data();

	obscure::vulkan::data_transfer_batch write_context{ device, transfer_pool };

	write_context.record_transfer(host_buffer, device_buffer, buffer_size);

	write_context.begin_transfers();

	write_context.wait_for_transfers();
}

obscure::vulkan::data_transfer_batch obscure::vulkan::staging_buffer::batched_write_data()&
{
	sync_data();

	obscure::vulkan::data_transfer_batch write_context{ device, transfer_pool };

	write_context.record_transfer(host_buffer, device_buffer, buffer_size);

	return write_context;
}

void obscure::vulkan::staging_buffer::batched_write_data(obscure::vulkan::data_transfer_batch& batch)&
{
	sync_data();

	batch.record_transfer(host_buffer, device_buffer, buffer_size);
}

obscure::vulkan::staging_buffer::~staging_buffer()
{
	if (mapped_memory)
	{
		vkUnmapMemory(device.get_handle(), host_buffer.get_memory_handle());
		host_buffer.free(device, allocator);
		device_buffer.free(device, allocator);
	}
}