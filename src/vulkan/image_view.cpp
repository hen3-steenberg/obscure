#include "obscure/vulkan/image_view.h"

obscure::vulkan::image_view::image_view() noexcept
	: vk_image_view(VK_NULL_HANDLE)
{}

obscure::vulkan::image_view::image_view(device _device, VkImage image, VkFormat image_format, VkAllocationCallbacks const* allocator)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

	createInfo.image = image;

	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = image_format;

	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	vkCreateImageView(_device.get_handle(), &createInfo, allocator, &vk_image_view);


}

void obscure::vulkan::image_view::free(device _device, VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyImageView(_device.get_handle(), vk_image_view, allocator);
}

VkImageView obscure::vulkan::image_view::get_handle() const noexcept
{
	return vk_image_view;
}