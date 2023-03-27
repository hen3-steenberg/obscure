#ifndef OBSCURE_VULKAN_CONFIGURATION_DEFINITION
#define OBSCURE_VULKAN_CONFIGURATION_DEFINITION 1
#include "obscure/vulkan/surface.h"
#include "obscure/vulkan/logger_collection.h"
namespace obscure
{
	namespace configuration
	{
		struct vulkan_configuration
		{
			virtual VkPhysicalDevice pick_device(vulkan::instance instance, vulkan::surface surface) = 0;

			virtual vulkan::logger_collection vulkan_logging(vulkan::instance instance, VkAllocationCallbacks const* allocator = nullptr) = 0;

			virtual ~vulkan_configuration() = default;
		};
	}
}
#endif