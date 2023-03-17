#include "obscure/vulkan/logger.h"
#include "obscure/color/virtual_terminal.h"
#include <iostream>

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

VkDebugUtilsMessengerEXT CreateUtilsMessenger(obscure::vulkan::instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes, void* pUserData)
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

obscure::vulkan::logger::logger(obscure::vulkan::instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes)
{
	obscure::vulkan::reference<VkDebugUtilsMessengerEXT>::set_value(CreateUtilsMessenger(instance, acceptedSeverity, acceptedMessageTypes, this), 
		[instance](VkDebugUtilsMessengerEXT logger) 
		{
			DestroyDebugUtilsMessengerEXT(instance.get_instance(), logger, nullptr); 
		});
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

static const auto severity = static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

static const auto messageTypes = static_cast<VkDebugUtilsMessageTypeFlagsEXT>(
	VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT);

obscure::vulkan::verbose_logger::verbose_logger(instance instance)
	: logger(instance, severity, messageTypes)
{}

obscure::vulkan::verbose_console_logger::verbose_console_logger(instance instance)
: verbose_logger(instance)
{}

VKAPI_ATTR VkBool32 VKAPI_CALL obscure::vulkan::verbose_console_logger::LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	std::cerr << virtual_terminal::grey_background;
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cerr << virtual_terminal::bright_cyan_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cerr << virtual_terminal::bright_green_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cerr << virtual_terminal::bright_yellow_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cerr << virtual_terminal::bright_red_text;
		break;
	}

	std::cerr << sw.elapsed_time().count() << " : " << type_text(messageType) << " : ";
	std::cerr << pCallbackData->pMessageIdName << " -> " << pCallbackData->pMessage;
	std::cerr << virtual_terminal::clear;
	std::cerr << "\n";

	return VK_FALSE;
}

obscure::vulkan::verbose_csv_logger::verbose_csv_logger(instance instance, const char* fileName)
: verbose_logger(instance), csv_file(fileName)
{}

VKAPI_ATTR VkBool32 VKAPI_CALL obscure::vulkan::verbose_csv_logger::LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	csv_file << sw.elapsed_time().count() << ", " << severity_text(messageSeverity) << ", " << type_text(messageType) << ", \"" << pCallbackData->pMessageIdName << "\", " << pCallbackData->messageIdNumber << ", \"" << pCallbackData->pMessage << "\"\n";
	csv_file.flush();

	return VK_FALSE;
}