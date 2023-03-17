#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/templates.hpp"

obscure::vulkan::swap_chain::swap_chain(device device)
{
	//VkSwapchainKHR swap_chain;
	//VkResult Err = vkCreateSwapchainKHR(device.get_handle(), &createInfo, nullptr, &swap_chain);
	//if (Err != VK_SUCCESS) throw Err;

	//set_value(swap_chain, [device](VkSwapchainKHR swap)
	//	{
	//		vkDestroySwapchainKHR(device.get_handle(), swap, nullptr);
	//	});

	//swap_chain_images = vulkan_load(device.get_handle(), swap_chain, vkGetSwapchainImagesKHR);
}