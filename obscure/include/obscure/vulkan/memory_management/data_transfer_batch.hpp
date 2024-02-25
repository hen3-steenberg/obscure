module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.data_transfer_batch;
import obscure.internal.vulkan.memory_owning_buffer;
import obscure.internal.vulkan.fence;
import obscure.internal.vulkan.command_pool;
import obscure.internal.vulkan.device;

namespace obscure
{
	namespace vulkan
	{
		export struct data_transfer_batch
		{
		protected:
			VkQueue transfer_queue;
			
			obscure::internal::vulkan::command_pool transfer_pool;
			obscure::internal::vulkan::fence transfer_completed;
			VkCommandBuffer transfer_commands;
		public:
			data_transfer_batch(obscure::internal::vulkan::device device, obscure::internal::vulkan::command_pool transfer_cmd_pool);

		
			data_transfer_batch(data_transfer_batch const& other) = delete;

			data_transfer_batch(data_transfer_batch&& other) noexcept;

			void record_transfer(obscure::internal::vulkan::memory_owning_buffer const& src, obscure::internal::vulkan::memory_owning_buffer const& dst, size_t size)&;

			void begin_transfers()&;

			void wait_for_transfers()&;

			~data_transfer_batch();
			
		};
	}
}