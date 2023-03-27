#include "obscure/vulkan/command_pool.h"

obscure::vulkan::command_pool::command_pool() noexcept
	: vk_command_pool(VK_NULL_HANDLE)
{}

obscure::vulkan::command_pool::command_pool(device device, uint32_t queue_family_index, VkAllocationCallbacks const* allocator)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.pNext = nullptr;

	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queue_family_index;
	

	vkCreateCommandPool(device.get_handle(), &poolInfo, allocator, &vk_command_pool);
}


VkCommandPool obscure::vulkan::command_pool::get_handle() const noexcept
{
	return vk_command_pool;
}
void obscure::vulkan::command_pool::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyCommandPool(device.get_handle(), vk_command_pool, allocator);
}

VkCommandBuffer obscure::vulkan::command_pool::allocate_primary_command_buffer(device device) const
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;

	allocInfo.commandPool = vk_command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer result{};

	vkAllocateCommandBuffers(device.get_handle(), &allocInfo, &result);

	return result;

}

void obscure::vulkan::command_pool::allocate_primary_command_buffers(device device, std::span<VkCommandBuffer> command_buffers) const
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;

	allocInfo.commandPool = vk_command_pool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

	vkAllocateCommandBuffers(device.get_handle(), &allocInfo, command_buffers.data());

}