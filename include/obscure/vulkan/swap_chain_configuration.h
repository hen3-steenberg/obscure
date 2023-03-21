#ifndef OBSCURE_VULKAN_SWAP_CHAIN_CONFIGURATION_DEFINITION
#define OBSCURE_VULKAN_SWAP_CHAIN_CONFIGURATION_DEFINITION 1
#include "surface.h"
#include <vector>
#include <memory>
namespace obscure
{
	namespace vulkan
	{
		struct swap_chain_configuration
		{
			VkPhysicalDevice device;
			surface vk_surface;
			VkSurfaceCapabilitiesKHR surface_capabilities;

			swap_chain_configuration(VkPhysicalDevice _device, surface _surface);

			virtual VkSurfaceFormatKHR format();

			virtual VkPresentModeKHR present_mode();

			virtual VkExtent2D extent();

			virtual uint32_t image_count();

			virtual bool meets_swap_chain_requirements();

			virtual VkSwapchainCreateInfoKHR parse_configuration(std::vector<uint32_t> const& queue_indices);

			static std::unique_ptr<swap_chain_configuration> make_configuration(VkPhysicalDevice _device, surface _surface);
		};
	}
}


#endif