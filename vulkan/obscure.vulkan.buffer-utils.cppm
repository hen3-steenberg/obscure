module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:utils;

export namespace obscure::vulkan
{
    template<VmaAllocationCreateFlags Flags>
    consteval bool
    can_write()
    {
        return Flags &
               (VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT);
    }

    template<VmaAllocationCreateFlags Flags>
    consteval bool
    can_read()
    {
        return Flags & (VMA_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT);
    }

    template<VmaAllocationCreateFlags Flags>
    consteval bool
    is_mapped()
    {
        return Flags & VMA_ALLOCATION_CREATE_MAPPED_BIT;
    }

    template<VkBufferUsageFlags Usage>
    consteval bool
    is_transfer_source()
    {
        return Usage & (VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    }

    template<VkBufferUsageFlags Usage>
    consteval bool
    is_transfer_destination()
    {
        return Usage & (VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }
} // namespace obscure::vulkan