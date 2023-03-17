#ifndef OBSCURE_VULKAN_DEVICE_PICKER_DEFINITION
#define OBSCURE_VULKAN_DEVICE_PICKER_DEFINITION 1

#include "instance.h"
#include <vector>


namespace obscure
{
	namespace vulkan
	{
		struct device_picker
		{
		protected:
			instance vk_instance;
		public:
			device_picker(instance instance);

			virtual std::vector<VkPhysicalDevice> load_devices();

			VkPhysicalDevice pick_device();

			virtual float score_device(VkPhysicalDevice device, VkPhysicalDeviceFeatures device_features, VkPhysicalDeviceProperties device_properties);
		};
	}
}
#endif