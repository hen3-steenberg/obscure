module;
#include <fstream>
#include "glfw_vulkan_include.h"
export module obscure.vulkan.verbose_csv_logger;
import obscure.stopwatch;
import obscure.vulkan.verbose_logger;

namespace obscure
{
	namespace vulkan
	{

		export struct verbose_csv_logger : verbose_logger
		{
			std::ofstream csv_file;
			stopwatch sw;
			verbose_csv_logger(obscure::internal::vulkan::instance instance, const char* fileName, VkAllocationCallbacks const* allocator = nullptr);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
			void free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}