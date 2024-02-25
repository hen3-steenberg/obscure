module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.verbose_console_logger;
import obscure.stopwatch;
import obscure.vulkan.verbose_logger;

namespace obscure
{
	namespace vulkan
	{

		export struct verbose_console_logger : verbose_logger
		{
			stopwatch sw;
			verbose_console_logger(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) override;
		};
	}
}