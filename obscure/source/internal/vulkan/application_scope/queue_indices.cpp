module;
#include <unordered_set>
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.queue_indices;
import obscure.internal.vulkan.vulkan_load;

obscure::internal::vulkan::queue_indices::queue_indices(VkPhysicalDevice device, surface surface)
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
	//if there is no unique transfer queue use the graphics queue
	transfer_index = graphics_index;
	//look for a unique transfer queue
	for (uint32_t index = 0; index < properties.size(); ++index)
	{
		if (index != graphics_index && index != present_index && (properties[index].queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT))
		{
			transfer_index = index;
			break;
		}
	}


}

std::vector<VkDeviceQueueCreateInfo> obscure::internal::vulkan::queue_indices::get_queue_create_infos() const
{
	std::unordered_set<uint32_t> indices = { graphics_index , present_index, transfer_index };
	std::vector<VkDeviceQueueCreateInfo> result;
	result.reserve(indices.size());
	for (uint32_t index : indices)
	{
		VkDeviceQueueCreateInfo index_info{};
		index_info.pNext = nullptr;
		index_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		index_info.flags = 0;
		index_info.queueCount = 1;
		index_info.pQueuePriorities = &priority;
		index_info.queueFamilyIndex = index;
		result.push_back(index_info);
	}
	return result;
}