module;
#include "glfw_vulkan_include.h"
export module obscure.vulkan.staged_array;
import obscure.vulkan.staging_buffer;

namespace obscure
{
	namespace vulkan
	{
		export template<typename TElem, VkBufferUsageFlags usage>
		struct staged_array : public staging_buffer
		{
		private:
			size_t _size;
		public:
			staged_array() noexcept
				: staging_buffer(), _size(0)
			{}

			staged_array(obscure::internal::vulkan::device _device, obscure::internal::vulkan::command_pool transfer_cmd_pool, size_t element_count, const VkAllocationCallbacks* _allocator = nullptr)
				: staging_buffer(_device, transfer_cmd_pool, sizeof(TElem) * element_count, usage, _allocator),
				_size(element_count)
			{}

			staged_array(staged_array const& other)
				: staging_buffer(other, usage), _size(other.size)
			{}

			staged_array(staged_array&& other) noexcept = default;

			staged_array& operator=(staged_array const& other) = default;
			staged_array& operator=(staged_array&& other) noexcept = default;

			inline size_t size() const&
			{
				return _size;
			}

			inline TElem* data()&
			{
				return reinterpret_cast<TElem*>(mapped_memory);
			}

			inline TElem const* data() const&
			{
				return reinterpret_cast<TElem const*>(mapped_memory);
			}

			inline TElem operator[](size_t index) const&
			{
				return data()[index];
			}

			inline TElem& operator[](size_t index)&
			{
				return data()[index];
			}

			inline TElem* begin() & noexcept
			{
				return data();
			}

			inline TElem* end() & noexcept
			{
				return data() + _size;
			}

			inline TElem const* begin() const& noexcept
			{
				return data();
			}

			inline TElem const* end() const& noexcept
			{
				return data() + _size;
			}
		};
	}
}