#ifndef OBSCURE_VULKAN_LOGGER_DEFINITION
#define OBSCURE_VULKAN_LOGGER_DEFINITION 1

#include "obscure/vulkan/instance.h"
#include "obscure/utilities/stopwatch.h"

#include <fstream>

namespace obscure
{
	namespace vulkan
	{
		struct logger
		{
			VkDebugUtilsMessengerEXT vk_logger;
			logger(instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes, VkAllocationCallbacks const* allocator = nullptr);

			virtual VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = 0;

			static inline const char* severity_text(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) noexcept;

			static inline const char* type_text(VkDebugUtilsMessageTypeFlagsEXT messageType) noexcept;

			virtual void free(instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};

		struct verbose_logger : logger
		{
			verbose_logger(instance instance, VkAllocationCallbacks const* allocator = nullptr);
		};

		struct verbose_console_logger : verbose_logger
		{
			stopwatch sw;
			verbose_console_logger(instance instance, VkAllocationCallbacks const* allocator = nullptr);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
		};

		struct verbose_csv_logger : verbose_logger
		{
			std::ofstream csv_file;
			stopwatch sw;
			verbose_csv_logger(instance instance, const char* fileName, VkAllocationCallbacks const* allocator = nullptr);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
			void free(instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;
		};
	}
}


#endif