#ifndef OBSCURE_VULKAN_VERTEX_ARRAY_DEFINITION
#define OBSCURE_VULKAN_VERTEX_ARRAY_DEFINITION 1
#include "obscure/vulkan/owning_buffer.h"
#include "obscure/vulkan/transfer_context.h"
#include <concepts>
namespace obscure
{
	namespace vulkan
	{
		struct render_context;

		template<typename TElem, VkBufferUsageFlags usage>
		struct buffer_array
		{
			friend render_context;
		private:
			size_t _size;
			memory_owning_mapped_staging_buffer host_buffer;
			memory_owning_device_buffer device_buffer;
		public:
			buffer_array() noexcept
				: _size(0), host_buffer(), device_buffer()
			{}

			buffer_array(vulkan::device _device, size_t size, VkPhysicalDeviceMemoryProperties properties, const VkAllocationCallbacks* allocator = nullptr)
				: _size(size), host_buffer(_device, sizeof(TElem)* size, properties, allocator), device_buffer(_device, sizeof(TElem) * size, usage, properties, allocator)
			{}

			buffer_array(buffer_array const& other) noexcept = default;
			buffer_array(buffer_array && other) noexcept = default;

			buffer_array& operator=(buffer_array const& other) noexcept = default;
			buffer_array& operator=(buffer_array&& other) noexcept = default;

			void transfer_to_device(transfer_context& ctx) &
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

			TElem* begin()& noexcept
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory);
			}

			TElem* end()& noexcept
			{
				return reinterpret_cast<TElem*>(host_buffer.mapped_memory) + _size;
			}

			TElem const* begin() const& noexcept
			{
				return reinterpret_cast<TElem const*>(host_buffer.mapped_memory);
			}

			TElem const* end() const& noexcept
			{
				return reinterpret_cast<TElem const*>(host_buffer.mapped_memory) + _size;
			}
		};

		template<typename TElem>
		using vertex_array = typename obscure::vulkan::buffer_array<TElem, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT>;

		template<VkIndexType TIndex>
		struct vulkan_index
		{};

		template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT8_EXT>
		{
			using type = typename uint8_t;
		};

		template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT16>
		{
			using type = typename uint16_t;
		};

		template<>
		struct vulkan_index<VK_INDEX_TYPE_UINT32>
		{
			using type = typename uint32_t;
		};

		template<VkIndexType TIndex>
		using vulkan_index_t = typename vulkan_index<TIndex>::type;

		template<VkIndexType TIndex>
		using index_array = typename obscure::vulkan::buffer_array<vulkan_index_t<TIndex>, VK_BUFFER_USAGE_INDEX_BUFFER_BIT>;

	}
}
#endif