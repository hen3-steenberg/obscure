#include "obscure/vulkan/graphics_queue.h"

obscure::vulkan::graphics_queue::graphics_queue()
{
	queue = VK_NULL_HANDLE;
}

obscure::vulkan::graphics_queue::graphics_queue(VkDevice device, uint32_t family_index)
{
	vkGetDeviceQueue(device, family_index, 0, &queue);
}