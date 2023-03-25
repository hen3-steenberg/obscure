#include "obscure/vulkan/render_pass.h"

obscure::vulkan::render_pass::render_pass() noexcept
	: vk_render_pass(VK_NULL_HANDLE)
{}

obscure::vulkan::render_pass::render_pass(device _device, VkRenderPassCreateInfo create_info, VkAllocationCallbacks const* allocator)
{
	vkCreateRenderPass(_device.get_handle(), &create_info, allocator, &vk_render_pass);
}

void obscure::vulkan::render_pass::free(device _device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyRenderPass(_device.get_handle(), vk_render_pass, allocator);
}

VkRenderPass obscure::vulkan::render_pass::get_handle() const noexcept
{
	return vk_render_pass;
}