#ifndef OBSCURE_VULKAN_VERTEX_ARRAY_DEFINITION
#define OBSCURE_VULKAN_VERTEX_ARRAY_DEFINITION 1
#include "obscure/vulkan/owning_buffer.h"
#include "obscure/vulkan/transfer_context.h"
namespace obscure
{
	namespace vulkan
	{
		struct render_context;

		template<typename TElem>
		struct vertex_array
		{
			friend render_context;
		private:
			size_t _size;
			memory_owning_mapped_staging_buffer host_buffer;
			memory_owning_vertex_buffer device_buffer;
		public:
			vertex_array() noexcept
				: _size(0), host_buffer(), device_buffer()
			{}

			vertex_array(vulkan::device _device, size_t size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr)
				: _size(sizeof(TElem)* size), host_buffer(_device, _size, properties, allocator), device_buffer(_device, _size, properties, allocator)
			{}

			vertex_array(vertex_array const& other) noexcept = default;
			vertex_array(vertex_array&& other) noexcept = default;

			vertex_array& operator=(vertex_array const& other) noexcept = default;
			vertex_array& operator=(vertex_array && other) noexcept = default;

			void transfer_to_device(transfer_context& ctx)
			{
				if (host_buffer.valid())
				{
					host_buffer.sync_memory();
					ctx.transfer(device_buffer, host_buffer);
				}
			}

			size_t size() const&
			{
				return _size;
			}

			TElem operator[](size_t index) const&
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory)[index];
			}

			TElem& operator[](size_t index)&
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory)[index];
			}

			TElem* begin()&
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory);
			}

			TElem* end()&
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory) + _size;
			}

			TElem const* begin() const&
			{
				return reinterpret_cast<TElem const*>(host_buffer.mapped_memory);
			}

			TElem const* end() const&
			{
				return reinterpret_cast<TElem const*>(host_buffer.mapped_memory) + _size;
			}
		};
	}
}
#endif