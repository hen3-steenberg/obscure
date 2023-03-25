#include <vector>
#include <array>
#include "obscure/vulkan/templates.hpp"
#include "obscure/properties.hpp"
#include "obscure/vulkan/glfw_api_context.h"
#include "obscure/vulkan/instance.h"

std::vector<const char*> LoadLayers()
{
	const std::array<const char*, 1> requested_layers = { "VK_LAYER_KHRONOS_validation" };

	std::vector<VkLayerProperties> properties = obscure::vulkan::vulkan_load(vkEnumerateInstanceLayerProperties);

	std::vector<const char*> res;
	for (auto const& p : properties)
	{
		for (const char* layer_name : requested_layers)
		{
			if (strcmp(p.layerName, layer_name) == 0)
			{
				res.push_back(layer_name);
				break;
			}
		}
	}
	return res;
}


std::span<const char*> obscure::vulkan::instance::GetAvailableLayers()
{
	if constexpr (obscure::enable_vulkan_validation_layers())
	{
		static std::vector<const char*> res = LoadLayers();
		return res;
	}
	else
	{
		static std::span<const char*> empty;
		return empty;
	}
}

std::span<const char*> obscure::vulkan::instance::GetRequiredDeviceExtensions()
{
	static constinit std::array<const char*,1> extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	return std::span<const char*>(extensions.data(), 1);
}

obscure::vulkan::instance::instance() noexcept
	: vk_instance(VK_NULL_HANDLE)
{}

obscure::vulkan::instance::instance(const char* AppName, version AppVersion, VkAllocationCallbacks const* allocator)
{
	constexpr const uint32_t obscure_ver = obscure::obscure_version().get_version();
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pEngineName = "obscure";
	appInfo.engineVersion = obscure_ver;
	appInfo.apiVersion = VK_API_VERSION_1_3;
	appInfo.pApplicationName = AppName;
	appInfo.applicationVersion = AppVersion.get_version();

	VkInstanceCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.flags = 0;

	auto available_layers = GetAvailableLayers();

	createInfo.enabledLayerCount = available_layers.size();
	createInfo.ppEnabledLayerNames = available_layers.data();

	auto extensions = glfw_api_contex::GetRequiredInstanceExtensions();
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkResult result = vkCreateInstance(&createInfo, allocator, &vk_instance);
	if (result != VK_SUCCESS)
		throw result;
}

void obscure::vulkan::instance::free(VkAllocationCallbacks const* allocator) noexcept
{
	vkDestroyInstance(vk_instance, allocator);
}

VkInstance obscure::vulkan::instance::get_instance() const noexcept
{
	return vk_instance;
}