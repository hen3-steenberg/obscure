module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.pipeline_layout;

obscure::internal::vulkan::pipeline_layout::pipeline_layout() noexcept
	: vk_layout(VK_NULL_HANDLE)
{}

obscure::internal::vulkan::pipeline_layout::pipeline_layout(device _device, VkPipelineLayoutCreateInfo create_info, VkAllocationCallbacks const* allocator)
{
	vkCreatePipelineLayout(_device.get_handle(), &create_info, allocator, &vk_layout);
}

void obscure::internal::vulkan::pipeline_layout::free(device _device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyPipelineLayout(_device.get_handle(), vk_layout, allocator);
}

VkPipelineLayout obscure::internal::vulkan::pipeline_layout::get_handle() const noexcept
{
	return vk_layout;
}