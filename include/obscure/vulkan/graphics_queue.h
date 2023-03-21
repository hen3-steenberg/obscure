#ifndef OBSCURE_VULKAN_GRAPHICS_QUEUE_DEFINITION
#define OBSCURE_VULKAN_GRAPHICS_QUEUE_DEFINITION 1
#include "glfw_vulkan_include.h"

namespace obscure
{
	namespace vulkan
	{
		struct graphics_queue
		{
			VkQueue queue;
			uint32_t queue_family_index;
			graphics_queue();
			graphics_queue(VkDevice device, uint32_t family_index);
		};
	}
}
#endif