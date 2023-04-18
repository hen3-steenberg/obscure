#ifndef OBSCURE_VULKAN_TRANSFER_CONTEXT_DEFINITION
#define OBSCURE_VULKAN_TRANSFER_CONTEXT_DEFINITION 1
#include "obscure/vulkan/owning_buffer.h"

namespace obscure
{
	namespace vulkan
	{
		struct transfer_context
		{
			VkCommandBuffer transfer_buffer;
			VkQueue transfer_queue;
			transfer_context(VkCommandBuffer _transfer_buffer, VkQueue _transfer_queue);

			transfer_context& transfer(memory_owning_device_buffer& destination, memory_owning_staging_buffer& src);
			transfer_context& transfer(memory_owning_device_buffer& destination, memory_owning_mapped_staging_buffer& src);
			void finalize_transfers();
		};
	}
}
#endif