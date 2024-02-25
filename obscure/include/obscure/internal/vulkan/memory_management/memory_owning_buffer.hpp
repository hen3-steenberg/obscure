module;
#include "glfw_vulkan_include.h"
export module obscure.internal.vulkan.memory_owning_buffer;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.device_memory;

namespace obscure
{
	namespace internal
	{
		namespace vulkan
		{
			export struct memory_owning_buffer
			{
				VkBuffer vk_buffer;
				device_memory memory;

				memory_owning_buffer() noexcept;
				memory_owning_buffer(device device, size_t size, VkBufferUsageFlags usage, VkMemoryPropertyFlags requested_properties, const VkAllocationCallbacks* _allocator = nullptr);
				memory_owning_buffer(memory_owning_buffer const& other) noexcept = default;

				memory_owning_buffer& operator=(memory_owning_buffer const& other) noexcept = default;


				VkBuffer get_handle() const& noexcept;
				VkDeviceMemory get_memory_handle() const& noexcept;

				void free(device device, const VkAllocationCallbacks* _allocator = nullptr) noexcept;
			};
		}
	}
}