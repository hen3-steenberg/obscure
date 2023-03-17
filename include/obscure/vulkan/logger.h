#ifndef OBSCURE_VULKAN_LOGGER_DEFINITION
#define OBSCURE_VULKAN_LOGGER_DEFINITION 1

#include "instance.h"
#include "reference.hpp"
#include "obscure/utilities/stopwatch.h"

#include <fstream>

namespace obscure
{
	namespace vulkan
	{
		struct logger : public reference<VkDebugUtilsMessengerEXT>
		{
			logger(instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes);

			virtual VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = 0;

			static inline const char* severity_text(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) noexcept;

			static inline const char* type_text(VkDebugUtilsMessageTypeFlagsEXT messageType) noexcept;
		};

		struct verbose_logger : logger
		{
			verbose_logger(instance instance);
		};

		struct verbose_console_logger : verbose_logger
		{
			stopwatch sw;
			verbose_console_logger(instance instance);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
		};

		struct verbose_csv_logger : verbose_logger
		{
			std::ofstream csv_file;
			stopwatch sw;
			verbose_csv_logger(instance instance, const char* fileName);
			VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData);
		};
	}
}


#endif