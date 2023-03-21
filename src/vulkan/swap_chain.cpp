#include "obscure/vulkan/swap_chain.h"
#include "obscure/vulkan/templates.hpp"
#include "obscure/vulkan/templates.hpp"
#include "obscure/vulkan/swap_chain_configuration.h"





obscure::vulkan::swap_chain::swap_chain(device device)
{
	std::unique_ptr<swap_chain_configuration> configuration = swap_chain_configuration::make_configuration(device.physical_device, device.vk_surface);

	std::vector<uint32_t> queue_families;
	queue_families.reserve(2);
	queue_families.push_back(device.g_queue.queue_family_index);
	if (device.g_queue.queue_family_index != device.p_queue.queue_family_index)
	{
		queue_families.push_back(device.p_queue.queue_family_index);
	}

	VkSwapchainCreateInfoKHR createInfo = configuration->parse_configuration(queue_families);

	image_format = createInfo.imageFormat;
	extent = createInfo.imageExtent;

	VkSwapchainKHR swap_chain;
	VkResult Err = vkCreateSwapchainKHR(device.get_handle(), &createInfo, nullptr, &swap_chain);
	if (Err != VK_SUCCESS) throw Err;

	set_value(swap_chain, [device](VkSwapchainKHR swap)
		{
			vkDestroySwapchainKHR(device.get_handle(), swap, nullptr);
		});

	swap_chain_images = vulkan_load(device.get_handle(), swap_chain, vkGetSwapchainImagesKHR);
}