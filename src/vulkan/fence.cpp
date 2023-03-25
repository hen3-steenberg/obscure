#include "obscure/vulkan/fence.h"

obscure::vulkan::fence::fence() noexcept
	: vk_fence(VK_NULL_HANDLE), vk_device(VK_NULL_HANDLE)
{}

obscure::vulkan::fence::fence(device device, bool state, VkAllocationCallbacks const* allocator)
	: vk_device(device.get_handle())
{
	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.pNext = nullptr;

	fence_info.flags = state ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

	vkCreateFence(vk_device, &fence_info, allocator, &vk_fence);
}

VkFence obscure::vulkan::fence::get_handle() const noexcept
{
	return vk_fence;
}

void obscure::vulkan::fence::wait()
{
	vkWaitForFences(vk_device, 1, &vk_fence, VK_TRUE, UINT64_MAX);
}
void obscure::vulkan::fence::reset()
{
	vkResetFences(vk_device, 1, &vk_fence);
}

void obscure::vulkan::fence::free(VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyFence(vk_device, vk_fence , allocator);
}