#include "obscure/vulkan/device_picker.h"
#include "obscure/vulkan/templates.hpp"
#include <system_error>

bool is_equal(VkExtensionProperties p, const char* name)
{
	return strcmp(p.extensionName, name) == 0;
}

obscure::vulkan::device_picker::device_picker(instance instance)
	: vk_instance(instance)
{}

std::vector<VkPhysicalDevice> obscure::vulkan::device_picker::load_devices()
{
	return vulkan_load(vk_instance.get_instance(), vkEnumeratePhysicalDevices);
}

VkPhysicalDevice obscure::vulkan::device_picker::pick_device()
{
	VkPhysicalDevice picked_device = VK_NULL_HANDLE;
	float picked_device_score = 0;
	for (auto device : load_devices())
	{
		VkPhysicalDeviceFeatures device_features;
		vkGetPhysicalDeviceFeatures(device, &device_features);
		VkPhysicalDeviceProperties device_properties;
		vkGetPhysicalDeviceProperties(device, &device_properties);
		float score = score_device(device, device_features, device_properties);
		if (score > picked_device_score)
		{
			picked_device = device;
			picked_device_score = score;
		}
	}
	if (picked_device_score)
	{
		return picked_device;
	}
	else throw std::errc::no_such_device;
}

float obscure::vulkan::device_picker::score_device(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties)
{
	float score = 0.0f;

	//prefer discrete gpu's
	if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	//larger images is better
	score += device_properties.limits.maxImageDimension2D;

	std::vector<VkExtensionProperties> availableExtensions = vulkan_load_data<VkExtensionProperties, VkResult, VkPhysicalDevice, const char*>(device, nullptr, vkEnumerateDeviceExtensionProperties);
	//needs the swap chain extension to work
	if (!contains_all(availableExtensions, instance::GetRequiredDeviceExtensions(), is_equal))
	{
		return -1;
	}
	//needs the geometry shader to work
	else if (!device_features.geometryShader) {
		return -1;
	}
	else return score;
}