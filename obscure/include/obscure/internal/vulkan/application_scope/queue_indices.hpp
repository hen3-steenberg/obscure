module;
#include <vector>
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.queue_indices;
import obscure.internal.vulkan.surface;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct queue_indices
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
}