module;
#include "glfw_vulkan_include.h"
export module obscure.configuration.vulkan_configuration;

import obscure.internal.vulkan.instance;
import obscure.internal.vulkan.surface;
import obscure.vulkan.logger_collection;

namespace obscure
{
	namespace configuration
	{
		export struct vulkan_configuration
		{
			virtual VkPhysicalDevice pick_device(obscure::internal::vulkan::instance instance, obscure::internal::vulkan::surface surface) = 0;

			virtual vulkan::logger_collection vulkan_logging(obscure::internal::vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) = 0;

			virtual ~vulkan_configuration() = default;
		};
	}
}