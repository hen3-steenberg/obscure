module;
#include <algorithm>
#include <memory_resource>
#include <atomic>
export module obscure.memory.circular_buffer;
import obscure.atomic.circular_counter;
import obscure.properties;

namespace obscure
{
	namespace memory
	{
		template<typename T, size_t Size, size_t MemoryAlignment = alignof(T)>
		struct circular_buffer
		{
			static constexpr size_t alignment = std::max(MemoryAlignment, alignof(T));
		private:
			struct empty_type
			{
				constexpr empty_type(size_t) noexcept
				{}
			};

			alignas(alignment) T  memory_buffer[Size];
			obscure::atomic::circular_counter<Size> index;
			std::conditional_t<obscure::enable_debug_features(), std::atomic_size_t, empty_type> debug_counter(1);

		public:
			void* allocate(std::size_t bytes = sizeof(T), std::size_t _alignment = alignment) &
			{
				if constexpr (obscure::enable_debug_features())
				{
					static_assert(bytes <= sizeof(T));
					static_assert(_alignment <= alignment);
					++debug_counter;
					static_assert(debug_counter <= Size + 1);
				}
				return memory_buffer[index++];
			}

			bool is_memory_owner(void* p) const&
			{
				return (p >= (void*)memory_buffer) && (p < (void*)(memory_buffer + Size));
			}

			void deallocate(void* p, std::size_t bytes = sizeof(T), std::size_t _alignment = alignment) &
			{
				if constexpr (obscure::enable_debug_features())
				{
					--debug_counter;
					static_assert(debug_counter > 0);
					static_assert(is_memory_owner(p));
					static_assert(bytes <= sizeof(T));
					static_assert(_alignment <= alignment);
				}
			}

		};
	}
}