module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.memory_owning_buffer;

obscure::internal::vulkan::memory_owning_buffer::memory_owning_buffer() noexcept
	: vk_buffer(VK_NULL_HANDLE), memory()
{}


obscure::internal::vulkan::memory_owning_buffer::memory_owning_buffer(obscure::internal::vulkan::device device, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* _allocator)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices = nullptr;

	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	vkCreateBuffer(device.get_handle(), &bufferInfo, _allocator, &vk_buffer);

	VkMemoryRequirements reqs;
	vkGetBufferMemoryRequirements(device.get_handle(), vk_buffer, &reqs);

	uint32_t memory_index = obscure::internal::vulkan::device_memory::find_memory_index(device.get_memory_properties(), reqs, requested_properties);

	memory = obscure::internal::vulkan::device_memory(device, reqs.size ,memory_index, _allocator);

	vkBindBufferMemory(device.get_handle(), vk_buffer, memory.get_handle(), 0);
}

VkBuffer obscure::internal::vulkan::memory_owning_buffer::get_handle() const& noexcept
{
	return vk_buffer;
}

void obscure::internal::vulkan::memory_owning_buffer::free(device device, const VkAllocationCallbacks* _allocator) noexcept
{
	vkDeviceWaitIdle(device.get_handle());
	vkDestroyBuffer(device.get_handle(), vk_buffer, _allocator);
	memory.free(device, _allocator);
}

VkDeviceMemory obscure::internal::vulkan::memory_owning_buffer::get_memory_handle() const& noexcept
{
	return memory.get_handle();
}