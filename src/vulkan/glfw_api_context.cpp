#include "obscure/vulkan/glfw_api_context.h"
#include "obscure/utilities/name_list.h"
#include "obscure/vulkan/glfw_vulkan_include.h"
#include "obscure/properties.hpp"

obscure::name_list LoadRequiredInstanceExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	if constexpr (obscure::enable_vulkan_validation_layers())
	{
		obscure::name_list res(glfwExtensionCount + 1);
		for (size_t index = 0; index < glfwExtensionCount; ++index)
		{
			res[index] = glfwExtensions[index];
		}
		res[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
		return res;
	}
	else
	{
		obscure::name_list res(glfwExtensions, glfwExtensionCount);
		return res;
	}

}

obscure::vulkan::glfw_api_contex::glfw_api_contex()
{
	glfwInit();
}

obscure::vulkan::glfw_api_contex::~glfw_api_contex()
{
	glfwTerminate();
}


std::span<const char*> obscure::vulkan::glfw_api_contex::GetRequiredInstanceExtensions()
{
	static const name_list extensions = LoadRequiredInstanceExtensions();
	return extensions;
}