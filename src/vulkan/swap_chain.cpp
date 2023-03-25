#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/templates.hpp"

 
obscure::vulkan::swap_chain::swap_chain() noexcept
	: vk_swap_chain(VK_NULL_HANDLE), swap_chain_images(), swap_chain_views(), image_format(VK_FORMAT_B8G8R8A8_SRGB), extent()
{}


obscure::vulkan::swap_chain::swap_chain(device device, VkSwapchainCreateInfoKHR create_info, VkAllocationCallbacks const* allocator)
{
	image_format = create_info.imageFormat;
	extent = create_info.imageExtent;

	VkResult Err = vkCreateSwapchainKHR(device.get_handle(), &create_info, nullptr, &vk_swap_chain);
	if (Err != VK_SUCCESS) throw Err;

	swap_chain_images = vulkan_load(device.get_handle(), vk_swap_chain, vkGetSwapchainImagesKHR);

	swap_chain_views.reserve(swap_chain_images.size());

	for (auto image : swap_chain_images)
	{
		swap_chain_views.emplace_back(device, image, image_format);
	}
}

VkSwapchainKHR obscure::vulkan::swap_chain::get_handle() const noexcept
{
	return vk_swap_chain;
}

void obscure::vulkan::swap_chain::free(device device, VkAllocationCallbacks const* allocator) noexcept
{

	for (auto image_view : swap_chain_views)
	{
		image_view.free(device, allocator);
	}

	vkDestroySwapchainKHR(device.get_handle(), vk_swap_chain, allocator);
}