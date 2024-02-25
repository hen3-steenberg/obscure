module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.swap_chain:configuration;
import obscure.internal.vulkan.surface;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{

			export struct swap_chain_configuration
			{
				VkPhysicalDevice physical_device;
				obscure::internal::vulkan::surface surface;
				VkExtent2D native_resolution;
				uint32_t queue_indices[2];
				VkSurfaceCapabilitiesKHR surface_capabilities;
				
				swap_chain_configuration(VkPhysicalDevice _physical_device, obscure::internal::vulkan::surface _surface,	VkExtent2D _native_resolution, uint32_t graphics_queue_index, uint32_t present_queue_index);

				virtual VkSurfaceFormatKHR format();

				virtual VkPresentModeKHR present_mode();

				virtual VkExtent2D extent();

				virtual uint32_t image_count();

				virtual VkSwapchainCreateInfoKHR parse_configuration();
			};
		}
	}
}