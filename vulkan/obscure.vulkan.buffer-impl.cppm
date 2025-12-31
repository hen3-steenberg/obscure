module;
#include "vma.hpp"
#include <vulkan/vulkan.hpp>
export module obscure.vulkan.buffer:impl;
export import obscure.vulkan.device;
import :utils;
import obscure.vulkan.result;

export namespace obscure::vulkan
{
    template<VkBufferUsageFlags Usage, VmaMemoryUsage MemoryUsage, VmaAllocationCreateFlags Flags, size_t Alignment>
    struct buffer_impl : vk::Buffer {
        [[nodiscard]] static consteval vk::BufferUsageFlags
        get_usage_flags() noexcept
        {
            return std::bit_cast<vk::BufferUsageFlags>(Usage);
        }

    private:
        static std::pair<vk::Buffer, VmaAllocation>
        create_buffer(device const& dev, size_t _size)
        {

            std::pair<vk::Buffer, VmaAllocation> result{ VK_NULL_HANDLE, nullptr };
            if (_size == 0) [[unlikely]] {
                return result;
            }

            VkBufferCreateInfo create_info = vk::BufferCreateInfo{ {}, _size, get_usage_flags() };

            VmaAllocationCreateInfo alloc_info{};
            alloc_info.usage = MemoryUsage;
            alloc_info.flags = Flags;

            auto* buffer_ptr = reinterpret_cast<VkBuffer*>(&result.first);

            if constexpr (Alignment == 1) {

                check(vmaCreateBuffer(
                    dev.get_vma_allocator(), &create_info, &alloc_info, buffer_ptr, &result.second, nullptr));
            } else {
                check(vmaCreateBufferWithAlignment(
                    dev.get_vma_allocator(), &create_info, &alloc_info, Alignment, buffer_ptr, &result.second, nullptr));
            }

            return result;
        }

        buffer_impl(const device& device, std::pair<vk::Buffer, VmaAllocation> buf, size_t size)
            : vk::Buffer(buf.first)
            , allocation(buf.second)
            , vk_device(&device)
            , _size(size)
        {
        }

    public:
        VmaAllocation allocation{};
        const device* vk_device;
        size_t _size;

        buffer_impl()
            : vk::Buffer(VK_NULL_HANDLE)
            , vk_device()
            , _size(0)
        {
        }

        buffer_impl(const device& device, size_t size) noexcept
            : buffer_impl(device, create_buffer(device, size), size)
        {
        }

        buffer_impl(buffer_impl const&) = delete;

        buffer_impl(buffer_impl&& other) noexcept
            : vk::Buffer(other)
            , allocation(other.allocation)
            , vk_device(other.vk_device)
            , _size(other._size)
        {
            other._size = 0;
        }

        buffer_impl& operator=(buffer_impl && other) = default;

        [[nodiscard]] vk::Buffer
        get_buffer() const noexcept
        {
            return static_cast<vk::Buffer>(*this);
        }

        [[nodiscard]] size_t
        size() const noexcept
        {
            return _size;
        }

        [[nodiscard]] vk::DeviceSize
        offset() const noexcept
        {
            VmaAllocationInfo info{};
            vmaGetAllocationInfo(vk_device->get_vma_allocator(), allocation, &info);

            return info.offset;
        }

        template<VmaAllocationCreateFlags Flags2 = Flags>
            requires(can_write<Flags>())
        void
        copy_data(void const* data, size_t bytes) const
        {
            vmaCopyMemoryToAllocation(vk_device->get_vma_allocator(), data, allocation, offset(), bytes);
        }

        template<typename T = void>
            requires(is_mapped<Flags>())
        [[nodiscard]] T*
        get_mapped_ptr() const
        {
            VmaAllocationInfo info{};
            vmaGetAllocationInfo(vk_device->get_vma_allocator(), allocation, &info);

            return std::bit_cast<T*>(info.pMappedData);
        }

        ~buffer_impl() noexcept
        {
            if (_size) {
                vk_device->waitIdle();
                vmaDestroyBuffer(vk_device->get_vma_allocator(), get_buffer(), allocation);
                _size = 0;
            }
        }

        operator bool() const noexcept { return _size != 0; }
    };
} // namespace obscure::vulkan
