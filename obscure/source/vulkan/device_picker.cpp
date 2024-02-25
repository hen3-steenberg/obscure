module;
#include <system_error>
#include "glfw_vulkan_include.h"
module obscure.vulkan.device_picker;
import obscure.internal.vulkan.vulkan_load;
import obscure.internal.contains_all;

bool is_equal(VkExtensionProperties p, const char* name)
{
	return strcmp(p.extensionName, name) == 0;
}

obscure::vulkan::device_picker::device_picker(obscure::internal::vulkan::instance instance, obscure::internal::vulkan::surface surface)
	: vk_instance(instance), vk_surface(surface)
{}

std::vector<VkPhysicalDevice> obscure::vulkan::device_picker::load_devices()
{
	return obscure::internal::vulkan::vulkan_load(vk_instance.get_instance(), vkEnumeratePhysicalDevices);
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

bool obscure::vulkan::device_picker::meets_minimum_requirements(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties)
{

	uint32_t count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface.get_handle(), &count, nullptr);
	if (count)
	{
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface.get_handle(), &count, nullptr);
	}

	if (count)
	{

		std::vector<VkExtensionProperties> availableExtensions = obscure::internal::vulkan::vulkan_load<VkPhysicalDevice, const char*, VkExtensionProperties, VkResult>(device, nullptr, vkEnumerateDeviceExtensionProperties);

		return obscure::internal::contains_all(availableExtensions, obscure::internal::vulkan::instance::GetRequiredDeviceExtensions(), is_equal) && device_features.geometryShader;
	}
	else return false;

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

	return score;
}