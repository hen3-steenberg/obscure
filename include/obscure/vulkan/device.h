#ifndef OBSCURE_VULKAN_DEVICE_DEFINITION
#define OBSCURE_VULKAN_DEVICE_DEFINITION 1

#include "surface.h"
#include "reference.hpp"
#include "graphics_queue.h"
#include "present_queue.h"

namespace obscure
{
	namespace vulkan
	{
		struct device : public reference<VkDevice>
		{
			graphics_queue g_queue;
			present_queue p_queue;
			VkPhysicalDevice physical_device;
			surface vk_surface;

			device(VkPhysicalDevice device, surface surface);
		};
	}
}
#endif