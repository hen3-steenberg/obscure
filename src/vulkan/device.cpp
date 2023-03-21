#include "obscure/vulkan/device.h"
#include "obscure/vulkan/queue_indices.h"

void free_device(VkDevice device)
{
	vkDestroyDevice(device, nullptr);
}

obscure::vulkan::device::device(VkPhysicalDevice device, surface surface)
	: physical_device(device), vk_surface(surface)
{

	VkPhysicalDeviceFeatures deviceFeatures{};

	queue_indices queue_info(device, surface);

	VkDeviceCreateInfo createInfo{};
	createInfo.pNext = nullptr;
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	auto queues = queue_info.get_queue_create_infos();
	createInfo.pQueueCreateInfos = queues.data();
	createInfo.queueCreateInfoCount = queues.size();

	createInfo.pEnabledFeatures = &deviceFeatures;

	auto validation_layers = instance::GetAvailableLayers();
	createInfo.enabledLayerCount = validation_layers.size();
	createInfo.ppEnabledLayerNames = validation_layers.data();

	auto extensions = instance::GetRequiredDeviceExtensions();
	createInfo.ppEnabledExtensionNames = extensions.data();
	createInfo.enabledExtensionCount = extensions.size();

	VkDevice logical_device;

	VkResult Err = vkCreateDevice(device, &createInfo, nullptr, &logical_device);
	if (Err != VK_SUCCESS) throw Err;

	set_value(logical_device, free_device);

	g_queue = graphics_queue(logical_device, queue_info.graphics_index);
	p_queue = present_queue(logical_device, queue_info.present_index);
}