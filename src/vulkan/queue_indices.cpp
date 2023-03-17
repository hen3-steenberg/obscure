#include "obscure/vulkan/queue_indices.h"
#include "obscure/vulkan/templates.hpp"

obscure::vulkan::queue_indices::queue_indices(VkPhysicalDevice device, surface surface)
{
	graphics_index = UINT32_MAX;
	present_index = UINT32_MAX;

	std::vector<VkQueueFamilyProperties> properties = vulkan_load(device, vkGetPhysicalDeviceQueueFamilyProperties);

	for (uint32_t index = 0; index < properties.size(); ++index)
	{
		if (properties[index].queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
		{
			graphics_index = index;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface.get_handle(), &presentSupport);
		if (presentSupport)
		{
			present_index = index;
		}

		if (graphics_index < properties.size() && present_index < properties.size()) break;
	}
}

std::vector<VkDeviceQueueCreateInfo> obscure::vulkan::queue_indices::get_queue_create_infos() const
{
	if (graphics_index == present_index)
	{
		std::vector<VkDeviceQueueCreateInfo> result(1);
		result[0].pNext = nullptr;
		result[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		result[0].queueFamilyIndex = graphics_index;
		result[0].queueCount = 1;
		result[0].pQueuePriorities = &priority;

		return result;
	}
	else
	{
		std::vector<VkDeviceQueueCreateInfo> result(2);

		result[0].pNext = nullptr;
		result[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		result[0].queueFamilyIndex = graphics_index;
		result[0].queueCount = 1;
		result[0].pQueuePriorities = &priority;

		result[1].pNext = nullptr;
		result[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		result[1].queueFamilyIndex = present_index;
		result[1].queueCount = 1;
		result[1].pQueuePriorities = &priority;

		return result;
	}

}