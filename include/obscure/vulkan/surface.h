#ifndef OBSCURE_VULKAN_SURFACE_DEFINITION
#define OBSCURE_VULKAN_SURFACE_DEFINITION 1

#include "reference.hpp"
#include "instance.h"
#include "window_reference.h"

namespace obscure
{
	namespace vulkan
	{
		struct surface : public reference<VkSurfaceKHR>
		{
			window_reference window;
			surface(instance instance, window_reference _window);
		};
	}
}
#endif