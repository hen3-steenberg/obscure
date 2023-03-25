#ifndef OBSCURE_VULKAN_CONFIGURATION_DEFINITION
#define OBSCURE_VULKAN_CONFIGURATION_DEFINITION 1
#include "obscure/vulkan/application_context.h"
#include "obscure/vulkan/logger_collection.h"
namespace obscure
{
	namespace configuration
	{
		struct vulkan_configuration
		{
			obscure::vulkan::application_context const* context;

			vulkan_configuration(obscure::vulkan::application_context const* ctx) noexcept
				: context(ctx)
			{}

			virtual VkPhysicalDevice pick_device() = 0;

			virtual VkSwapchainCreateInfoKHR configure_swap_chain() = 0;

			virtual vulkan::logger_collection vulkan_logging(VkAllocationCallbacks const* allocator = nullptr) = 0;

			virtual ~vulkan_configuration() = default;
		};
	}
}
#endif