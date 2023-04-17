#ifndef OBSCURE_VULKAN_QUEUE_INDICES_DEFINITION
#define OBSCURE_VULKAN_QUEUE_INDICES_DEFINITION 1

#include "obscure/vulkan/surface.h"
#include <vector>
namespace obscure
{
	namespace vulkan
	{
		struct queue_indices
		{
			static constexpr float priority = 1.0f;
			uint32_t graphics_index;
			uint32_t present_index;
			uint32_t transfer_index;

			queue_indices(VkPhysicalDevice device, surface surface);

			std::vector<VkDeviceQueueCreateInfo> get_queue_create_infos() const;
		};
	}
}
#endif