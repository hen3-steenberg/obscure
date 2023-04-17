#ifndef OBSCURE_VULKAN_BUFFER_MEMORY_DEFINITION
#define OBSCURE_VULKAN_BUFFER_MEMORY_DEFINITION 1
#include "obscure/vulkan/device_memory.h"
namespace obscure
{
	namespace vulkan
	{
		struct buffer_memory
		{
			void* mapped_memory;
			vulkan::device device;
			vulkan::device_memory device_memory;

			buffer_memory(vulkan::device _device, vulkan::device_memory _memory, size_t size);
			~buffer_memory();

			template<typename T>
			T& at(size_t index)&;

			template<typename T>
			T const& at(size_t index) const&;
		};
	}
}
#endif