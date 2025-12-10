module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:staging;
import :impl;

export namespace obscure::vulkan
{
    template<typename T>
    using staging_buffer_impl =
        buffer_impl<VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
                    VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                    std::alignment_of_v<T>>;

    template<typename T>
    struct staging_buffer : public staging_buffer_impl<T> {
        size_t _count;
        T* _data;
        staging_buffer(const device& device, size_t count)
            : staging_buffer_impl<T>(device, count * sizeof(T))
            , _count(count)
            , _data(staging_buffer_impl<T>::template get_mapped_ptr<T>())
        {
        }

        T*
        data() noexcept
        {
            return _data;
        }

        T const*
        data() const noexcept
        {
            return _data;
        }

        size_t
        count() const noexcept
        {
            return _count;
        }

        T&
        operator[](size_t idx) noexcept
        {
            return _data[idx];
        }

        T const&
        operator[](size_t idx) const noexcept
        {
            return _data[idx];
        }

        T*
        begin() noexcept
        {
            return _data;
        }

        T const*
        begin() const noexcept
        {
            return _data;
        }

        T*
        end() noexcept
        {
            return _data + _count;
        }

        T const*
        end() const noexcept
        {
            return _data + _count;
        }
    };
} // namespace obscure::vulkan