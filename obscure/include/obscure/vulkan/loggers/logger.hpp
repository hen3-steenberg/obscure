module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.logger;
import obscure.internal.vulkan.instance;

namespace obscure
{
	namespace vulkan
	{
		export struct logger
		{
			VkDebugUtilsMessengerEXT vk_logger;
			logger(obscure::internal::vulkan::instance instance, VkDebugUtilsMessageSeverityFlagBitsEXT acceptedSeverity, VkDebugUtilsMessageTypeFlagsEXT acceptedMessageTypes, VkAllocationCallbacks const* allocator = nullptr);

			virtual VKAPI_ATTR VkBool32 VKAPI_CALL LogEvent(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) = 0;

			static inline const char* severity_text(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) noexcept;

			static inline const char* type_text(VkDebugUtilsMessageTypeFlagsEXT messageType) noexcept;

			virtual void free(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) noexcept;

			virtual ~logger() = default;
		};
	}
}