module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:index;
import :impl;

export namespace obscure::vulkan
{
    template<typename T>
    concept vk_index = std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>;

    template<vk_index TIndex>
    using index_buffer_impl = buffer_impl<VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                          VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
                                          0,
                                          std::alignment_of_v<TIndex>>;

    template<vk_index TIndex>
    struct index_buffer : index_buffer_impl<TIndex> {
        size_t _count;

        static consteval vk::IndexType
        get_index_type()
        {
            if constexpr (std::is_same_v<TIndex, uint8_t>) {
                return vk::IndexType::eUint8;
            } else if constexpr (std::is_same_v<TIndex, uint16_t>) {
                return vk::IndexType::eUint16;
            } else {
                return vk::IndexType::eUint32;
            }
        }

        index_buffer(const device& device, size_t count)
            : index_buffer_impl<TIndex>(device, count * sizeof(TIndex))
            , _count(count)
        {
        }

        [[nodiscard]] size_t
        count() const noexcept
        {
            return _count;
        }
    };
} // namespace obscure::vulkan