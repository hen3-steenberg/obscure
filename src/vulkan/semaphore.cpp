#include "obscure/vulkan/semaphore.h"

obscure::vulkan::semaphore::semaphore() noexcept
	: vk_semaphore(VK_NULL_HANDLE)
{}

obscure::vulkan::semaphore::semaphore(device device, VkAllocationCallbacks const* allocator)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = nullptr;
	semaphoreInfo.flags = 0;

	vkCreateSemaphore(device.get_handle(), &semaphoreInfo, allocator, &vk_semaphore);
}

VkSemaphore obscure::vulkan::semaphore::get_handle() const noexcept
{
	return vk_semaphore;
}

VkSemaphore const* obscure::vulkan::semaphore::get_handle_address() const noexcept
{
	return &vk_semaphore;
}
void obscure::vulkan::semaphore::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroySemaphore(device.get_handle(), vk_semaphore, allocator);
}