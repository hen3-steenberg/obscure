module;
#include <compare>
#include "glfw_vulkan_include.h"
module obscure.vulkan.verbose_csv_logger;

obscure::vulkan::verbose_csv_logger::verbose_csv_logger(obscure::internal::vulkan::instance instance, const char* fileName, VkAllocationCallbacks const* allocator)
	: verbose_logger(instance, allocator), csv_file(fileName)
{}

VKAPI_ATTR VkBool32 VKAPI_CALL obscure::vulkan::verbose_csv_logger::LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData)
{
	csv_file << sw.elapsed_time().count() << ", " << severity_text(messageSeverity) << ", " << type_text(messageType) << ", \"" << pCallbackData->pMessageIdName << "\", " << pCallbackData->messageIdNumber << ", \"" << pCallbackData->pMessage << "\"\n";
	csv_file.flush();

	return VK_FALSE;
}

void obscure::vulkan::verbose_csv_logger::free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator) noexcept
{
	obscure::vulkan::logger::free(instance, allocator);
	csv_file.close();
}