module;
#include <vector>
#include "glfw_vulkan_include.h"
export module obscure.vulkan.device_picker;
import obscure.internal.vulkan.instance;
import obscure.internal.vulkan.surface;

namespace obscure
{
	namespace vulkan
	{
		export struct device_picker
		{
		protected:
			obscure::internal::vulkan::instance vk_instance;
			obscure::internal::vulkan::surface vk_surface;

			virtual std::vector<VkPhysicalDevice> load_devices();

			virtual bool meets_minimum_requirements(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties);

			virtual float score_device(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties);
		public:
			device_picker(obscure::internal::vulkan::instance instance, obscure::internal::vulkan::surface surface);

			VkPhysicalDevice pick_device();

		};
	}
}