module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.staging_buffer;
import obscure.internal.vulkan.memory_owning_buffer;
import obscure.internal.vulkan.device;
import obscure.internal.vulkan.command_pool;
import obscure.vulkan.data_transfer_batch;


namespace obscure
{
	namespace vulkan
	{
		export struct staging_buffer
		{
		protected:
			obscure::internal::vulkan::memory_owning_buffer host_buffer;
			obscure::internal::vulkan::memory_owning_buffer device_buffer;
			obscure::internal::vulkan::device device;
			void* mapped_memory;
			VkAllocationCallbacks const* allocator;
			size_t buffer_size;
			obscure::internal::vulkan::command_pool transfer_pool;
			inline void sync_data()&;
		public:
			staging_buffer() noexcept;
			staging_buffer(obscure::internal::vulkan::device _device, obscure::internal::vulkan::command_pool transfer_cmd_pool,  size_t _size, VkBufferUsageFlags buffer_usage, const VkAllocationCallbacks* _allocator = nullptr);
			staging_buffer(staging_buffer const& other, VkBufferUsageFlags buffer_usage);
			staging_buffer(staging_buffer && other) noexcept;
		
		
			void write_data()&;

			data_transfer_batch batched_write_data()&;

			void batched_write_data(data_transfer_batch& batch)&;

			~staging_buffer();
		};


	}
}