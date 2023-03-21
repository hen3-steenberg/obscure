#ifndef PRESENT_QUEUE_DEFINITION
#define PRESENT_QUEUE_DEFINITION 1
#include "glfw_vulkan_include.h"

namespace obscure
{
	namespace vulkan
	{
		struct present_queue
		{
			VkQueue queue;
			uint32_t queue_family_index;
			present_queue();
			present_queue(VkDevice device, uint32_t family_index);
		};
	}
}
#endif