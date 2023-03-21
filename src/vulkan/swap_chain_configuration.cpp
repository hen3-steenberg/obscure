#include "obscure/vulkan/swap_chain_configuration.h"
#include "obscure/vulkan/templates.hpp"
#include <vector>
#include <limits>
#include <algorithm>

obscure::vulkan::swap_chain_configuration::swap_chain_configuration(VkPhysicalDevice _device, obscure::vulkan::surface _surface)
	: device(_device), vk_surface(_surface)
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface.get_handle(), &surface_capabilities);
}

std::unique_ptr<obscure::vulkan::swap_chain_configuration> obscure::vulkan::swap_chain_configuration::make_configuration(VkPhysicalDevice _device, obscure::vulkan::surface _surface)
{
	return std::make_unique<obscure::vulkan::swap_chain_configuration>(_device, _surface);
}


bool obscure::vulkan::swap_chain_configuration::meets_swap_chain_requirements()
{
	uint32_t count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface.get_handle(), &count, nullptr);
	if (count)
	{
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface.get_handle(), &count, nullptr);
	}
	return count != 0;
}

VkSurfaceFormatKHR obscure::vulkan::swap_chain_configuration::format()
{
	std::vector<VkSurfaceFormatKHR> formats = obscure::vulkan::vulkan_load(device, vk_surface.get_handle(), vkGetPhysicalDeviceSurfaceFormatsKHR);
	for (const auto& availableFormat : formats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return formats[0];
}

VkPresentModeKHR obscure::vulkan::swap_chain_configuration::present_mode()
{
	std::vector<VkPresentModeKHR> present_modes = obscure::vulkan::vulkan_load(device, vk_surface.get_handle(), vkGetPhysicalDeviceSurfacePresentModesKHR);
	for (const auto& availablePresentMode : present_modes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D obscure::vulkan::swap_chain_configuration::extent()
{
#undef max
	if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surface_capabilities.currentExtent;
	}
	else {
		VkExtent2D result = vk_surface.window.get_native_resolution();

		result.width = std::clamp(result.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
		result.height = std::clamp(result.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

		return result;
	}
}

uint32_t obscure::vulkan::swap_chain_configuration::image_count()
{
	uint32_t result = surface_capabilities.minImageCount + 1;
	if (surface_capabilities.maxImageCount && result < surface_capabilities.maxImageCount)
	{
		return surface_capabilities.maxImageCount;
	}
	else return result;
}

VkSwapchainCreateInfoKHR obscure::vulkan::swap_chain_configuration::parse_configuration(std::vector<uint32_t> const& queue_indices)
{
	VkSwapchainCreateInfoKHR result;
	result.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	result.pNext = nullptr;
	result.flags = 0;

	result.surface = vk_surface.get_handle();

	result.minImageCount = image_count();

	VkSurfaceFormatKHR surface_format = format();
	result.imageFormat = surface_format.format;
	result.imageColorSpace = surface_format.colorSpace;

	result.imageExtent = extent();

	result.imageArrayLayers = 1;

	result.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	result.preTransform = surface_capabilities.currentTransform;

	result.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	result.presentMode = present_mode();

	result.clipped = VK_TRUE;

	result.oldSwapchain = VK_NULL_HANDLE;

	

	if (queue_indices.size() <= 1)
	{
		result.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		result.queueFamilyIndexCount = 0; // Optional
		result.pQueueFamilyIndices = nullptr; // Optional
	}
	else if (queue_indices.size() > 1)
	{
		result.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		result.queueFamilyIndexCount = queue_indices.size();
		result.pQueueFamilyIndices = queue_indices.data();
	}

	

	return result;
}