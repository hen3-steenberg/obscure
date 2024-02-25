module;
#include <cstring>
#include "glfw_vulkan_include.h"
export module obscure.vulkan.mapped_uniform;
import obscure.vulkan.mapped_buffer;

namespace obscure
{
	namespace vulkan
	{
		template<typename T>
		struct mapped_uniform : public mapped_buffer
		{
			mapped_uniform(obscure::internal::vulkan::device _device, const VkAllocationCallbacks* _allocator = nullptr)
				: mapped_buffer(_device, sizeof(T), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, _allocator)
			{}

			mapped_uniform(mapped_uniform const& other)
				: mapped_buffer(other, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			{}

			mapped_uniform(mapped_uniform&& other) noexcept = default;

			mapped_uniform& operator=(mapped_uniform const& other) = default;
			mapped_uniform& operator=(mapped_uniform&& other) noexcept = default;

			mapped_uniform& operator=(T const& other)
			{
				std::memcpy(mapped_memory, &other, sizeof(T));
				return* this;
			}

			inline T* data()&
			{
				return reinterpret_cast<T*>(mapped_memory);
			}

			inline T const* data() const&
			{
				return reinterpret_cast<T const*>(mapped_memory);
			}

			inline T* operator->()&
			{
				return data();
			}

			inline T const* operator->() const&
			{
				return data();
			}

			explicit operator T() const&
			{
				T result;
				std::memcpy(&result, mapped_memory, sizeof(T));
				return result;
			}

			explicit operator T& ()&
			{
				return *data();
			}

			explicit operator T const& ()const&
			{
				return *data();
			}


		};
	}
}