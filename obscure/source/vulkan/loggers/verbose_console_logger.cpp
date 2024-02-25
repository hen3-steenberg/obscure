module;
#include <iostream>
#include "glfw_vulkan_include.h"
module obscure.vulkan.verbose_console_logger;
import obscure.color.virtual_terminal;

obscure::vulkan::verbose_console_logger::verbose_console_logger(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator)
	: verbose_logger(instance, allocator)
{}

VKAPI_ATTR VkBool32 VKAPI_CALL obscure::vulkan::verbose_console_logger::LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	std::cerr << obscure::color::virtual_terminal::grey_background;
	switch (messageSeverity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cerr << obscure::color::virtual_terminal::bright_cyan_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cerr << obscure::color::virtual_terminal::bright_green_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cerr << obscure::color::virtual_terminal::bright_yellow_text;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cerr << obscure::color::virtual_terminal::bright_red_text;
		break;
		//included to satisfy -Wswitch
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
		break;
	}

	std::cerr << sw.elapsed_time().count() << " : " << type_text(messageType) << " : ";
	std::cerr << pCallbackData->pMessageIdName << " -> " << pCallbackData->pMessage;
	std::cerr << obscure::color::virtual_terminal::clear;
	std::cerr << "\n";

	return VK_FALSE;
}