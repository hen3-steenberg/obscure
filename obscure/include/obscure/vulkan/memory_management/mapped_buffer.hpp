module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.mapped_buffer;
import obscure.internal.vulkan.memory_owning_buffer;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace vulkan
	{
		export struct mapped_buffer
		{
		protected:
			obscure::internal::vulkan::memory_owning_buffer buffer;
			obscure::internal::vulkan::device device;
			void* mapped_memory;
			VkAllocationCallbacks const* allocator;
			size_t buffer_size;
		public:
			mapped_buffer() noexcept;
			mapped_buffer(obscure::internal::vulkan::device _device, size_t _size, VkBufferUsageFlags buffer_usage, const VkAllocationCallbacks* _allocator = nullptr);
			mapped_buffer(mapped_buffer const& other, VkBufferUsageFlags buffer_usage);
			mapped_buffer(mapped_buffer&& other) noexcept;
			void write_data()&;
			void read_data()&;
			~mapped_buffer();
		};
	}
}