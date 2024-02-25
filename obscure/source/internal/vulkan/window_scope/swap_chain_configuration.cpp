module;
#include <vector>
#include <algorithm>
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.swap_chain:configuration;
import obscure.internal.vulkan.surface;
import obscure.internal.vulkan.vulkan_load;

obscure::internal::vulkan::swap_chain_configuration::swap_chain_configuration(VkPhysicalDevice _physical_device, obscure::internal::vulkan::surface _surface, VkExtent2D _native_resolution, uint32_t graphics_queue_index, uint32_t present_queue_index)
	: physical_device(_physical_device), surface(_surface), native_resolution(_native_resolution), queue_indices{graphics_queue_index, present_queue_index}
{
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface.get_handle(), &surface_capabilities);
}

VkSurfaceFormatKHR obscure::internal::vulkan::swap_chain_configuration::format()
{
	std::vector<VkSurfaceFormatKHR> formats = obscure::internal::vulkan::vulkan_load(physical_device, surface.get_handle(), vkGetPhysicalDeviceSurfaceFormatsKHR);
	for (const auto& availableFormat : formats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return formats[0];
}

VkPresentModeKHR obscure::internal::vulkan::swap_chain_configuration::present_mode()
{
	std::vector<VkPresentModeKHR> present_modes = obscure::internal::vulkan::vulkan_load(physical_device, surface.get_handle(), vkGetPhysicalDeviceSurfacePresentModesKHR);
	for (const auto& availablePresentMode : present_modes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D obscure::internal::vulkan::swap_chain_configuration::extent()
{
#undef max
	if (surface_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return surface_capabilities.currentExtent;
	}
	else {
		VkExtent2D result = native_resolution;

		result.width = std::clamp(result.width, surface_capabilities.minImageExtent.width, surface_capabilities.maxImageExtent.width);
		result.height = std::clamp(result.height, surface_capabilities.minImageExtent.height, surface_capabilities.maxImageExtent.height);

		return result;
	}
}

uint32_t obscure::internal::vulkan::swap_chain_configuration::image_count()
{
	uint32_t result = surface_capabilities.minImageCount + 1;
	if (surface_capabilities.maxImageCount && result < surface_capabilities.maxImageCount)
	{
		return surface_capabilities.maxImageCount;
	}
	else return result;
}

VkSwapchainCreateInfoKHR obscure::internal::vulkan::swap_chain_configuration::parse_configuration()
{
	uint32_t queue_index_count = 1;

	if (queue_indices[0] != queue_indices[1]) queue_index_count = 2;

	VkSwapchainCreateInfoKHR result{};
	result.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	result.pNext = nullptr;
	result.flags = 0;

	result.surface = surface.get_handle();

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



	if (queue_index_count <= 1)
	{
		result.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		result.queueFamilyIndexCount = 0; // Optional
		result.pQueueFamilyIndices = nullptr; // Optional
	}
	else if (queue_index_count > 1)
	{
		result.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		result.queueFamilyIndexCount = queue_index_count;
		result.pQueueFamilyIndices = queue_indices;
	}



	return result;
}