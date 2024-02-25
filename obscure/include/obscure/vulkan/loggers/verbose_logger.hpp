module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.verbose_logger;

import obscure.vulkan.logger;

namespace obscure
{
	namespace vulkan
	{
		export struct verbose_logger : public logger
		{
			static const auto any_severity = static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);

			static const auto all_message_types = static_cast<VkDebugUtilsMessageTypeFlagsEXT>(
				VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT);

			verbose_logger(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr);
		};
	}
}