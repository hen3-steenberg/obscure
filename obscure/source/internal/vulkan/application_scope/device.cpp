module;
#include "glfw_vulkan_include.h"
module obscure.internal.vulkan.device;

import obscure.internal.vulkan.queue_indices;

obscure::internal::vulkan::device::device() noexcept
	: vk_device(VK_NULL_HANDLE), physical_device(VK_NULL_HANDLE), graphics_queue_index(-1), present_queue_index(-1), transfer_queue_index(-1)
{}

obscure::internal::vulkan::device::device(VkPhysicalDevice device, obscure::internal::vulkan::surface surface, VkAllocationCallbacks const* allocator)
	: physical_device(device)
{

	VkPhysicalDeviceFeatures deviceFeatures{};

	obscure::internal::vulkan::queue_indices queue_info(device, surface);

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

	VkResult Err = vkCreateDevice(device, &createInfo, allocator, &vk_device);
	if (Err != VK_SUCCESS) throw Err;

	graphics_queue_index = queue_info.graphics_index;
	present_queue_index = queue_info.present_index;
	transfer_queue_index = queue_info.transfer_index;
}

void obscure::internal::vulkan::device::free(VkAllocationCallbacks const* allocator) & noexcept
{
	vkDestroyDevice(vk_device, allocator);
}

VkDevice obscure::internal::vulkan::device::get_handle() const& noexcept
{
	return vk_device;
}

VkQueue obscure::internal::vulkan::device::get_graphics_queue() const&
{
	VkQueue result;
	vkGetDeviceQueue(vk_device, graphics_queue_index, 0, &result);
	return result;
}

VkQueue obscure::internal::vulkan::device::get_present_queue() const&
{
	VkQueue result;
	vkGetDeviceQueue(vk_device, graphics_queue_index, 0, &result);
	return result;
}

VkQueue obscure::internal::vulkan::device::get_transfer_queue() const&
{
	VkQueue result;
	vkGetDeviceQueue(vk_device, transfer_queue_index, 0, &result);
	return result;
}

void obscure::internal::vulkan::device::wait_for_idle() &
{
	vkDeviceWaitIdle(vk_device);
}

VkPhysicalDeviceMemoryProperties obscure::internal::vulkan::device::get_memory_properties() const&
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memProperties);
	return memProperties;
}