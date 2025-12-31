module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:vertex;
import :impl;

export namespace obscure::vulkan
{
    template<typename T>
    using vertex_buffer_impl = buffer_impl<VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                           VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                                           0,
                                           std::alignment_of_v<T>>;

    template<typename T>
    struct vertex_buffer : vertex_buffer_impl<T> {
        size_t _count;

        vertex_buffer()
            : vertex_buffer_impl<T>(),
              _count(0)
        {}

        vertex_buffer(const device& device, size_t count)
            : vertex_buffer_impl<T>(device, count * sizeof(T))
            , _count(count)
        {
        }

        vertex_buffer(vertex_buffer && other)
            : vertex_buffer_impl<T>(std::move(other)),
              _count(other._count)
        {
            other._count = 0;
        }

        vertex_buffer& operator=(vertex_buffer && other) = default;



        [[nodiscard]] size_t
        count() const noexcept
        {
            return _count;
        }
    };
} // namespace obscure::vulkan
