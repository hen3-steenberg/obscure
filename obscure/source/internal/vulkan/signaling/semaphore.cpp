module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.semaphore;

obscure::internal::vulkan::semaphore::semaphore() noexcept
	: vk_semaphore(VK_NULL_HANDLE)
{}

obscure::internal::vulkan::semaphore::semaphore(device device, VkAllocationCallbacks const* allocator)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreInfo.pNext = nullptr;
	semaphoreInfo.flags = 0;

	vkCreateSemaphore(device.get_handle(), &semaphoreInfo, allocator, &vk_semaphore);
}

VkSemaphore obscure::internal::vulkan::semaphore::get_handle() const noexcept
{
	return vk_semaphore;
}

VkSemaphore const* obscure::internal::vulkan::semaphore::get_handle_address() const noexcept
{
	return &vk_semaphore;
}
void obscure::internal::vulkan::semaphore::free(device device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroySemaphore(device.get_handle(), vk_semaphore, allocator);
}