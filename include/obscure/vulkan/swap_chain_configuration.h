#ifndef OBSCURE_VULKAN_SWAP_CHAIN_CONFIGURATION_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_CONFIGURATION_DEFINITION 1
#include "obscure/vulkan/glfw_vulkan_include.h"
#include <vector>
#include <memory>
namespace obscure
{
	namespace vulkan
	{
		struct application_context;

		struct swap_chain_configuration
		{
			application_context const* ctx;
			VkSurfaceCapabilitiesKHR surface_capabilities;
			swap_chain_configuration(application_context const * context);

			virtual VkSurfaceFormatKHR format();

			virtual VkPresentModeKHR present_mode();

			virtual VkExtent2D extent();

			virtual uint32_t image_count();

			virtual VkSwapchainCreateInfoKHR parse_configuration();
		};
	}
}


#endif