#ifndef OBSCURE_VULKAN_DEVICE_PICKER_DEFINITION
#define OBSCURE_VULKAN_DEVICE_PICKER_DEFINITION 1

#include "obscure/vulkan/surface.h"
#include <vector>


namespace obscure
{
	namespace vulkan
	{
		struct device_picker
		{
		protected:
			instance vk_instance;
			surface vk_surface;
		public:
			device_picker(instance instance, surface surface);

			virtual std::vector<VkPhysicalDevice> load_devices();

			VkPhysicalDevice pick_device();

			virtual bool meets_minimum_requirements(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties);

			virtual float score_device(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties);
		};
	}
}
#endif