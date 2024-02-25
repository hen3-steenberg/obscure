module;
#include "glfw_vulkan_include.h"
module obscure.vulkan.logger;

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (pUserData)
	{
		return reinterpret_cast<obscure::vulkan::logger*>(pUserData)->LogEvent(messageSeverity, messageType, pCallbackData);
	}
	else
	{
		return VK_FALSE;
	}
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

VkDebugUtilsMessengerEXT CreateUtilsMessenger(obscure::internal::vulkan::instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes, void* pUserData)
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.pNext = nullptr;
	createInfo.messageSeverity = acceptedSeverity;
	createInfo.messageType = acceptedMessageTypes;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = pUserData;

	VkDebugUtilsMessengerEXT logMessenger;
	VkResult Err = CreateDebugUtilsMessengerEXT(instance.get_instance(), &createInfo, nullptr, &logMessenger);
	if (Err != VK_SUCCESS) throw Err;
	return logMessenger;
}

obscure::vulkan::logger::logger(obscure::internal::vulkan::instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes, VkAllocationCallbacks const* allocator)
{
	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.pNext = nullptr;
	createInfo.messageSeverity = acceptedSeverity;
	createInfo.messageType = acceptedMessageTypes;
	createInfo.pfnUserCallback = debugCallback;
	createInfo.pUserData = this;


	VkResult Err = CreateDebugUtilsMessengerEXT(instance.get_instance(), &createInfo, allocator, &vk_logger);
	if (Err != VK_SUCCESS) throw Err;
}

void obscure::vulkan::logger::free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator) noexcept
{
	DestroyDebugUtilsMessengerEXT(instance.get_instance(), vk_logger, allocator);
}

const char* obscure::vulkan::logger::severity_text(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) noexcept
{
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		return "VERBOSE";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		return "INFO";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		return "WARNING";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

const char* obscure::vulkan::logger::type_text(VkDebugUtilsMessageTypeFlagsEXT messageType) noexcept
{
	switch (messageType)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
		return "DEVICE_ADDRESS_BINDING";
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		return "GENERAL";
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		return "PERFORMANCE";
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		return "VALIDATION";
	default:
		return "UNKNOWN";
	}
}